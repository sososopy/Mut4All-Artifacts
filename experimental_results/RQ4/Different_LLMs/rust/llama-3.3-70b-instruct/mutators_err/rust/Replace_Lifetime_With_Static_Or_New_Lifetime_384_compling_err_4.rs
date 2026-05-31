use rand::Rng;
use rand::thread_rng;
use syn::{parse_quote, FnArg, Lifetime, ReferenceType, ReturnType, Span, Type};
use syn::visit_mut::VisitMut;

struct Replace_Lifetime_With_Static_Or_New_Lifetime_384;

impl Replace_Lifetime_With_Static_Or_New_Lifetime_384 {
    fn name(&self) -> &str {
        "Replace_Lifetime_With_Static_Or_New_Lifetime_384"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                let mut rng = thread_rng();
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Reference(ref_type) = &*pat_type.ty {
                            if let Some(lifetime) = &ref_type.lifetime {
                                let new_lifetime = if rng.gen_bool(0.5) {
                                    Lifetime::new("'static", Span::call_site())
                                } else {
                                    Lifetime::new("'new", Span::call_site())
                                };
                                let mut new_ref_type = ref_type.clone();
                                new_ref_type.lifetime = Some(new_lifetime);
                                pat_type.ty = Box::new(Type::Reference(new_ref_type));
                            }
                        }
                    }
                }
                if let ReturnType::Type(_, return_type) = &func.sig.output {
                    if let Type::Reference(ref_type) = &**return_type {
                        if let Some(lifetime) = &ref_type.lifetime {
                            let new_lifetime = if rng.gen_bool(0.5) {
                                Lifetime::new("'static", Span::call_site())
                            } else {
                                Lifetime::new("'new", Span::call_site())
                            };
                            let mut new_ref_type = ref_type.clone();
                            new_ref_type.lifetime = Some(new_lifetime);
                            *return_type = Box::new(Type::Reference(new_ref_type));
                        }
                    }
                }
            }
            if let syn::Item::Impl(impl_item) = item {
                for impl_item in &mut impl_item.items {
                    if let syn::ImplItem::Fn(method) = impl_item {
                        if method.sig.ident == "main" {
                            continue;
                        }
                        let mut rng = thread_rng();
                        for input in &mut method.sig.inputs {
                            if let FnArg::Typed(pat_type) = input {
                                if let Type::Reference(ref_type) = &*pat_type.ty {
                                    if let Some(lifetime) = &ref_type.lifetime {
                                        let new_lifetime = if rng.gen_bool(0.5) {
                                            Lifetime::new("'static", Span::call_site())
                                        } else {
                                            Lifetime::new("'new", Span::call_site())
                                        };
                                        let mut new_ref_type = ref_type.clone();
                                        new_ref_type.lifetime = Some(new_lifetime);
                                        pat_type.ty = Box::new(Type::Reference(new_ref_type));
                                    }
                                }
                            }
                        }
                        if let ReturnType::Type(_, return_type) = &method.sig.output {
                            if let Type::Reference(ref_type) = &**return_type {
                                if let Some(lifetime) = &ref_type.lifetime {
                                    let new_lifetime = if rng.gen_bool(0.5) {
                                        Lifetime::new("'static", Span::call_site())
                                    } else {
                                        Lifetime::new("'new", Span::call_site())
                                    };
                                    let mut new_ref_type = ref_type.clone();
                                    new_ref_type.lifetime = Some(new_lifetime);
                                    *return_type = Box::new(Type::Reference(new_ref_type));
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces lifetimes of references in function signatures with either a static lifetime or a new lifetime. This transformation tests the compiler's handling of lifetime relationships and borrow checking in the context of generic const expressions and impl trait."
    }
}