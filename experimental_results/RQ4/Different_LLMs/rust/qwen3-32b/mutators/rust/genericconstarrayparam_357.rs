use syn::parse_quote;
use crate::mutator::Mutator;
use std::collections::HashSet;
use syn::Item;
use syn::Type;
use syn::ImplItem;
use syn::ReturnType;
use syn::Expr;
use syn::Stmt;

pub struct GenericConstArrayParam_357;

impl Mutator for GenericConstArrayParam_357 {
    fn name(&self) -> &str {
        "GenericConstArrayParam_357"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut modified_structs = HashSet::new();

        for item in &mut file.items {
            if let Item::Struct(s) = item {
                let has_array = s.fields.iter().any(|f| {
                    if let Type::Array(_) = &f.ty {
                        true
                    } else {
                        false
                    }
                });
                if has_array {
                    let generic = parse_quote!(const N: usize);
                    s.generics.params.push(generic);
                    modified_structs.insert(s.ident.clone());
                    for field in &mut s.fields {
                        if let Type::Array(array) = &mut field.ty {
                            array.len = parse_quote!(N);
                        }
                    }
                }
            } else if let Item::Enum(e) = item {
                let has_array = e.variants.iter().any(|variant| {
                    variant.fields.iter().any(|f| {
                        if let Type::Array(_) = &f.ty {
                            true
                        } else {
                            false
                        }
                    })
                });
                if has_array {
                    let generic = parse_quote!(const N: usize);
                    e.generics.params.push(generic);
                    modified_structs.insert(e.ident.clone());
                    for variant in &mut e.variants {
                        for field in &mut variant.fields {
                            if let Type::Array(array) = &mut field.ty {
                                array.len = parse_quote!(N);
                            }
                        }
                    }
                }
            }
        }

        for item in &mut file.items {
            if let Item::Impl(impl_item) = item {
                if let Type::Path(self_ty_path) = &*impl_item.self_ty {
                    if let Some(name) = modified_structs.iter().find(|n| {
                        if let Some(seg) = self_ty_path.path.segments.iter().next() {
                            seg.ident == **n
                        } else {
                            false
                        }
                    }) {
                        let generic = parse_quote!(const N: usize);
                        impl_item.generics.params.push(generic);
                        for impl_item in &mut impl_item.items {
                            if let ImplItem::Fn(method) = impl_item {
                                if let ReturnType::Type(_, return_type) = &method.sig.output {
                                    if let Type::ImplTrait(_) = **return_type {
                                        let expr: Expr = parse_quote! { self.0.iter().take(N) };
                                        method.block.stmts.push(Stmt::Expr(expr, None));
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
        ""
    }
}