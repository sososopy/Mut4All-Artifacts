use syn::{parse_quote, visit_mut::VisitMut, Type, TypePath, Path, PathArguments, PathSegment, GenericArgument, Lifetime, punctuated::Punctuated};
use crate::mutator::Mutator;

struct Swap_Generic_Lifetime_Argument_341;

impl Mutator for Swap_Generic_Lifetime_Argument_341 {
    fn name(&self) -> &str {
        "Swap_Generic_Lifetime_Argument_341"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Path(TypePath {
                        path: Path { segments, .. },
                        ..
                    }) = &**return_type {
                        for segment in segments {
                            if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                let mut generic_args: Vec<GenericArgument> = args.args.iter().cloned().collect();
                                let mut lifetime_args: Vec<Lifetime> = func.sig.generics.lifetimes().map(|lt| lt.lifetime.clone()).collect();
                                if !generic_args.is_empty() && !lifetime_args.is_empty() {
                                    let generic_arg = generic_args.remove(0);
                                    let lifetime_arg = lifetime_args.remove(0);
                                    generic_args.insert(0, GenericArgument::Lifetime(lifetime_arg));
                                }
                            }
                        }
                    }
                }
            }
            if let syn::Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let syn::ImplItem::Fn(func) = impl_item {
                        if let syn::ReturnType::Type(_, return_type) = &func.sig.output {
                            if let Type::Path(TypePath {
                                path: Path { segments, .. },
                                ..
                            }) = &**return_type {
                                for segment in segments {
                                    if let PathArguments::AngleBracketed(args) = &segment.arguments {
                                        let mut generic_args: Vec<GenericArgument> = args.args.iter().cloned().collect();
                                        let mut lifetime_args: Vec<Lifetime> = func.sig.generics.lifetimes().map(|lt| lt.lifetime.clone()).collect();
                                        if !generic_args.is_empty() && !lifetime_args.is_empty() {
                                            let generic_arg = generic_args.remove(0);
                                            let lifetime_arg = lifetime_args.remove(0);
                                            generic_args.insert(0, GenericArgument::Lifetime(lifetime_arg));
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
        "The mutation operator swaps a generic argument with a lifetime argument in a generic type, trait, or function definition. This transformation can help expose bugs in the Rust compiler related to generic const expressions and lifetime arguments."
    }
}