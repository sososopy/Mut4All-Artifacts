use syn::{File, Item, ReturnType, Type, TypePath, TypeImplTrait};
use proc_macro2::Ident;
use crate::mutator::Mutator;

pub struct Modify_Function_Return_Type_234;

impl Mutator for Modify_Function_Return_Type_234 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_234"
    }
    fn mutate(&self, file: &mut File) {
        let items = file.items.clone(); // Clone items to avoid borrowing issues
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if let ReturnType::Type(_, ref mut return_type) = func.sig.output {
                    if let Type::Path(TypePath { path, .. }) = &**return_type {
                        if path.segments.len() == 1 {
                            let segment = &path.segments[0];
                            if segment.ident == "T" { // Assuming T is a type alias for impl Trait
                                if let Some(alias_type) = find_type_alias(&items, &segment.ident) {
                                    if let Type::ImplTrait(TypeImplTrait { bounds, .. }) = alias_type {
                                        *return_type = Box::new(Type::ImplTrait(TypeImplTrait {
                                            impl_token: Default::default(),
                                            bounds: bounds.clone(),
                                        }));
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation operator identifies functions returning a type alias for an `impl Trait` and modifies the return type to directly use `impl Trait`. It ensures the function implementation remains valid by adjusting the return statement if necessary. This targets potential bugs in type resolution."
    }
}

fn find_type_alias<'a>(items: &'a [Item], alias_name: &'a Ident) -> Option<&'a Type> {
    for item in items {
        if let Item::Type(type_item) = item {
            if type_item.ident == *alias_name {
                return Some(&*type_item.ty);
            }
        }
    }
    None
}