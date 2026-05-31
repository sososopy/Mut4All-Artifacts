use crate::mutator::Mutator;

pub struct Remove_Impl_Trait_Lifetime_Params_193;

impl Mutator for Remove_Impl_Trait_Lifetime_Params_193 {
    fn name(&self) -> &str {
        "Remove_Impl_Trait_Lifetime_Params_193"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Impl(impl_item) = item {
                if let Some((_, trait_path, _)) = &mut impl_item.trait_ {
                    let mut has_lifetime_args = false;
                    for segment in &trait_path.segments {
                        if let syn::PathArguments::AngleBracketed(args) = &segment.arguments {
                            if args.args.iter().any(|arg| matches!(arg, syn::GenericArgument::Lifetime(_))) {
                                has_lifetime_args = true;
                                break;
                            }
                        }
                    }
                    if has_lifetime_args {
                        let has_async_method = impl_item.items.iter().any(|impl_item| {
                            if let syn::ImplItem::Fn(method) = impl_item {
                                method.sig.asyncness.is_some()
                            } else {
                                false
                            }
                        });
                        if has_async_method {
                            for segment in &mut trait_path.segments {
                                if let syn::PathArguments::AngleBracketed(args) = &mut segment.arguments {
                                    args.args = args.args.clone().into_iter()
                                        .filter(|arg| !matches!(arg, syn::GenericArgument::Lifetime(_)))
                                        .collect();
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator removes explicit lifetime parameters from trait implementation paths when the trait has async methods with lifetime bounds and is defined in a different module/crate. This forces the compiler to infer lifetimes in async contexts, potentially exposing bugs in cross-crate trait resolution and lifetime elision rules."
    }
}