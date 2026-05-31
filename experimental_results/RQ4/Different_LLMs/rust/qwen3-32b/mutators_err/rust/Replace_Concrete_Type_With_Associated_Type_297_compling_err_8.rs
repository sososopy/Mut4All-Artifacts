use crate::mutator::Mutator;
use syn::parse_quote;
use syn::FnArg;
use syn::Type;
use syn::ReturnType;
use syn::Item;
use syn::ImplItem;
use syn::ItemImpl;

pub struct Replace_Concrete_Type_With_Associated_Type_297;

impl Mutator for Replace_Concrete_Type_With_Associated_Type_297 {
    fn name(&self) -> &str {
        "Replace_Concrete_Type_With_Associated_Type_297"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Fn(func) = item {
                if func.sig.constness.is_some() {
                    for input in &mut func.sig.inputs {
                        if let FnArg::Typed(pat_type) = input {
                            if let Type::Path(_) = &**pat_type.ty {
                                *pat_type.ty = parse_quote!(<Self as UnknownTrait>::AssocType);
                            }
                        }
                    }
                    if let ReturnType::Type(_, ty) = &mut func.sig.output {
                        if let Type::Path(_) = &**ty {
                            *ty = parse_quote!(<Self as UnknownTrait>::AssocType);
                        }
                    }
                }
            } else if let Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let ImplItem::Fn(func) = impl_item {
                        if func.sig.constness.is_some() {
                            for input in &mut func.sig.inputs {
                                if let FnArg::Typed(pat_type) = input {
                                    if let Type::Path(_) = &**pat_type.ty {
                                        *pat_type.ty = parse_quote!(<Self as UnknownTrait>::AssocType);
                                    }
                                }
                            }
                            if let ReturnType::Type(_, ty) = &mut func.sig.output {
                                if let Type::Path(_) = &**ty {
                                    *ty = parse_quote!(<Self as UnknownTrait>::AssocType);
                                }
                            }
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