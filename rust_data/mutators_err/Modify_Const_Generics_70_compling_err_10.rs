use crate::mutator::Mutator;
use syn::{parse_quote, Item, GenericArgument, Expr, Ident, GenericParam};

pub struct Modify_Const_Generics_70;

impl Mutator for Modify_Const_Generics_70 {
    fn name(&self) -> &str {
        "Modify_Const_Generics_70"
    }
    
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                for item in &mut trait_item.items {
                    if let syn::TraitItem::Fn(method) = item {
                        if let Some(syn::FnArg::Typed(pat_type)) = method.sig.inputs.first() {
                            if let syn::Type::Reference(type_ref) = &*pat_type.ty {
                                if let syn::Type::Path(type_path) = &*type_ref.elem {
                                    let mut segments = type_path.path.segments.clone();
                                    if let Some(segment) = segments.last_mut() {
                                        if segment.ident == "usize" {
                                            if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                                if let Some(GenericArgument::Const(expr)) = args.args.first_mut() {
                                                    if let Expr::Path(expr_path) = expr {
                                                        if let Some(ident) = expr_path.path.get_ident() {
                                                            let new_ident = Ident::new("M", ident.span());
                                                            expr_path.path.segments.last_mut().unwrap().ident = new_ident;
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    }
                                    let new_type_path = syn::TypePath {
                                        qself: type_path.qself.clone(),
                                        path: syn::Path {
                                            leading_colon: type_path.path.leading_colon,
                                            segments,
                                        },
                                    };
                                    *type_ref.elem = Box::new(syn::Type::Path(new_type_path));
                                }
                            }
                        }
                    }
                }
            }

            if let Item::Struct(struct_item) = item {
                for param in &mut struct_item.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if const_param.ty == parse_quote!(usize) {
                            const_param.ident = Ident::new("M", const_param.ident.span());
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generics within traits and structs. It modifies the const generic parameter identifiers in traits and structs, creating a mismatch between the declaration and usage. This tests the compiler's handling of const generic parameters, potentially uncovering inconsistencies or bugs in the const generics implementation."
    }
}