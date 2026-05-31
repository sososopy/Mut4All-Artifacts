use syn::{File, Item, ItemFn, ReturnType, Type, TypeReference, GenericParam, Lifetime};
use crate::mutator::Mutator;
use syn::spanned::Spanned;

struct Modify_Lifetime_Parameters_In_Async_Return_Types_18;

impl Mutator for Modify_Lifetime_Parameters_In_Async_Return_Types_18 {
    fn name(&self) -> &str {
        "Modify_Lifetime_Parameters_In_Async_Return_Types_18"
    }

    fn mutate(&self, file: &mut File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.asyncness.is_some() {
                    if let ReturnType::Type(_, return_type) = &func.sig.output {
                        if let Type::Reference(type_reference) = &**return_type {
                            if let Some(lifetime) = &type_reference.lifetime {
                                let mut new_lifetime: Option<Lifetime> = None;
                                for param in &func.sig.generics.params {
                                    if let GenericParam::Lifetime(lifetime_param) = param {
                                        new_lifetime = Some(lifetime_param.lifetime.clone());
                                        break;
                                    }
                                }
                                if let Some(new_lifetime) = new_lifetime {
                                    let new_type_reference = TypeReference {
                                        and_token: type_reference.and_token.clone(),
                                        lifetime: Some(new_lifetime),
                                        mutability: type_reference.mutability.clone(),
                                        elem: type_reference.elem.clone(),
                                    };
                                    let new_return_type = Type::Reference(new_type_reference);
                                    func.sig.output = ReturnType::Type(func.sig.output.span(), Box::new(Type::Verbatim(quote::quote!(-> #new_return_type).into())));
                                } else {
                                    let new_type_reference = TypeReference {
                                        and_token: type_reference.and_token.clone(),
                                        lifetime: None,
                                        mutability: type_reference.mutability.clone(),
                                        elem: type_reference.elem.clone(),
                                    };
                                    let new_return_type = Type::Reference(new_type_reference);
                                    func.sig.output = ReturnType::Type(func.sig.output.span(), Box::new(Type::Verbatim(quote::quote!(-> #new_return_type).into())));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets async functions with return types containing references to opaque types. It modifies the lifetime parameters of the return type, changing them to other valid lifetime parameters present in the function signature, or removing them altogether if possible. This transformation aims to test the compiler's handling of lifetime parameters in async function return types."
    }
}