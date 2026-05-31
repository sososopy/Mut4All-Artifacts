use syn::{self, MacroDelimiter};
use crate::mutator::Mutator;

pub struct Modify_Macro_Delimiters_230;

impl Mutator for Modify_Macro_Delimiters_230 {
    fn name(&self) -> &str {
        "Modify_Macro_Delimiters_230"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(item_macro) = item {
                match &mut item_macro.mac.delimiter {
                    MacroDelimiter::Brace(_) => {
                        item_macro.mac.delimiter = MacroDelimiter::Paren(Default::default());
                    }
                    MacroDelimiter::Paren(_) => {
                        item_macro.mac.delimiter = MacroDelimiter::Bracket(Default::default());
                    }
                    MacroDelimiter::Bracket(_) => {
                        item_macro.mac.delimiter = MacroDelimiter::Brace(Default::default());
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro definitions and modifies their delimiters to introduce mismatched delimiters. This transformation aims to provoke parsing errors or internal compiler errors related to delimiter mismatches, testing the compiler's robustness in handling macro patterns and expansions with incorrect delimiters."
    }
}