use crate::audio_file::AudioFile;

#[derive(Debug, Clone)]
pub struct CircularQueue {
    pub rear: usize, 
    pub size: usize,
    pub capacity: usize,
    pub items: Vec<AudioFile>
}


impl CircularQueue {

    pub fn new(capacity: usize) -> Self {
        CircularQueue {
            rear: 0, 
            size: 0,
            capacity: capacity,
            items: Vec::new(),
        }
    }

    pub fn enqueue(&mut self, audio_file: AudioFile) -> bool {
        if self.is_full() {
            println!("Full"); 
            return false; 
        }        

        self.items.push(audio_file);
        self.rear = (self.rear + 1) & self.items.len();
        self.size += 1;
        true
    }

    pub fn dequeue(&mut self) -> bool {
        if self.is_empty() {
            return false; 
        }

        self.items.pop();
        self.size -= 1; 
        true
    }

    pub fn is_full(&self) -> bool {
        self.size == self.capacity
    }

    pub fn is_empty(&self) -> bool {
        self.size == 0
    }

}


#[cfg(test)]
mod queue_library {
 
    use crate::audio_file::AudioFile;
    use crate::queue::CircularQueue; 

    #[test]
    fn test_create_queue()  {

        /* create instance of queue */ 
        let capacity = 11; 
        let mut queue = CircularQueue::new(capacity);

        assert_eq!(queue.rear, 0); 
        assert_eq!(queue.size, 0);
        assert_eq!(queue.items.len(), 0);  

        /* insert 5 dummy audio files */ 
        for i in 0..9 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file : AudioFile = AudioFile::new(&file_value, "mp3", "storage_path", 1000, 2);
            queue.enqueue(my_file); 
        }

        assert_eq!(queue.items.len(), 9);  
    }


    #[test]
    fn test_queue_indexing()  {

        /* create instance of queue */ 
        let capacity = 11; 
        let mut queue = CircularQueue::new(capacity);

        /* insert 5 dummy audio files */
        let mut expected_rear_index = 1;  
        for i in 0..9 {
            let file_value = format!("audio_file_{}", i);
            let my_file : AudioFile = AudioFile::new(&file_value, "mp3", "storage_path", 1000, 2);
            queue.enqueue(my_file);

            /* ensure rear indexing is working */  
            assert_eq!(queue.rear, expected_rear_index); 
            expected_rear_index += 1; 
        }

        assert_eq!(queue.items.len(), 9); 
        
    }

    #[test]
    fn test_queue_items() {

        /* create instance of queue */ 
        let capacity = 11; 
        let mut queue = CircularQueue::new(capacity);

        /* insert 5 dummy audio files */
        for i in 0..9 {
            let file_value = format!("audio_file_{}", i);
            let mut my_file : AudioFile = AudioFile::new(&file_value, "mp3", "storage_path", 1000, 2);
            queue.enqueue(my_file);
        }

        queue.dequeue();
        queue.dequeue(); 

        /* iterate through queue and check items */
        let mut expected_index = 0;  
        for item in queue.items {
            let file_value = format!("audio_file_{}", expected_index);
            assert_eq!(file_value, item.file_name); 
            expected_index += 1; 
        }

        assert_eq!(expected_index, 7);
    }    



}