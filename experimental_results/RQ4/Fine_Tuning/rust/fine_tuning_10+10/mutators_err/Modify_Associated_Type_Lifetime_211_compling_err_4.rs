use crate::mutator::Mutator;
use syn::{File, Item, Type, TypePath, PathArguments, GenericArgument, ImplItem};

pub struct Modify_Associated_Type_Lifetime_211;

impl Mutator for Modify_Associated_Type_Lifetime_211 {
    fn name(&self) -> &str {
        "Modify_Associated_Type_Lifetime_211"
    }
    
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Impl(impl_block) = item {
                for item in &mut impl_block.items {
                    if let ImplItem::Type(type_item) = item {
                        if let Type::Path(TypePath { path, .. }) = &mut type_item.ty {
                            if let Some(last_segment) = path.segments.last_mut() {
                                if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                                    for arg in &mut args.args {
                                        if let GenericArgument::Lifetime(lifetime) = arg {
                                            lifetime.ident = Ident::new("d", lifetime.ident.span());
                                        }
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
        "This mutation operator identifies associated types within impl blocks and modifies their lifetime parameters to deliberately introduce conflicts. By changing the lifetime to a conflicting one, it aims to uncover potential lifetime-related issues, which are common triggers for internal compiler errors (ICEs) during constraint resolution."
    }
}