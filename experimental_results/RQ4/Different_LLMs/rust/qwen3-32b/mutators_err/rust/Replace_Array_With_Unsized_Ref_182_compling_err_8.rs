use crate::mutator::Mutator;
use syn::{File, Item, ItemStruct, Type, TypeArray, TypeReference, Mutability};

pub struct Replace_Array_With_Unsized_Ref_182;

impl Mutator for Replace_Array_With_Unsized_Ref_182 {
    fn name(&self) -> &str {
        "Replace_Array_With_Unsized_Ref_182"
    }
    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Struct(struct_item) = item {
                let struct_name = &struct_item.ident;

                // Check if there's a const fn that returns this struct
                let has_const_fn = file.items.iter().any(|i| {
                    if let Item::Fn(fn_item) = i {
                        if fn_item.sig.constness.is_some() {
                            if let syn::ReturnType::Type(_, ty) = &fn_item.sig.output {
                                if let Type::Path(type_path) = &**ty {
                                    if let Some(ident) = type_path.path.get_ident() {
                                        if ident == struct_name {
                                            return true;
                                        }
                                    }
                                }
                            }
                        }
                        false
                    } else {
                        false
                    }
                });

                // Check if there's a static mut declaration of this struct
                let has_static_mut = file.items.iter().any(|i| {
                    if let Item::Static(static_item) = i {
                        if let Some(Mutability::Mut(_)) = static_item.mutability {
                            if let Type::Path(type_path) = &*static_item.ty {
                                if let Some(ident) = type_path.path.get_ident() {
                                    if ident == struct_name {
                                        return true;
                                    }
                                }
                            }
                            true
                        } else {
                            false
                        }
                    } else {
                        false
                    }
                });

                if has_const_fn && has_static_mut {
                    for field in &mut struct_item.fields {
                        if let Type::Array(array) = &mut field.ty {
                            // Create a reference to the element type
                            let ref_type = Type::Reference(TypeReference {
                                and_token: syn::Token![&](proc_macro2::Span::call_site()),
                                lifetime: None,
                                mutability: None,
                                elem: array.elem.clone(),
                            });
                            array.elem = Box::new(ref_type);
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}