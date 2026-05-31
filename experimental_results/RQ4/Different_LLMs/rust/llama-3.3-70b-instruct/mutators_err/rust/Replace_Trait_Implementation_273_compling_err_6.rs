use crate::mutator::Mutator;
use syn::{File, Item, ItemImpl, Path, PathSegment, punctuated::Punctuated, token::For};
use proc_macro2::{Ident, Span};

struct Replace_Trait_Implementation_273;

impl Mutator for Replace_Trait_Implementation_273 {
    fn name(&self) -> &str {
        "Replace_Trait_Implementation_273"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &item_impl.trait_ {
                    let new_trait_path = Path {
                        leading_colon: path.leading_colon,
                        segments: {
                            let mut segments = Punctuated::new();
                            segments.push(PathSegment {
                                ident: Ident::new(
                                    &format!("{}Modified", path.segments.first().unwrap().ident),
                                    Span::call_site(),
                                ),
                                arguments: path.segments.first().unwrap().arguments.clone(),
                            });
                            segments
                        },
                    };
                    item_impl.trait_ = Some((For::None, new_trait_path, For::None));
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces the trait implementation with a modified version. It renames the trait by prefixing 'Modified' to the original trait name, potentially changing the behavior of the program. This transformation aims to test the compiler's ability to resolve trait implementations and handle changes to the trait system."
    }
}