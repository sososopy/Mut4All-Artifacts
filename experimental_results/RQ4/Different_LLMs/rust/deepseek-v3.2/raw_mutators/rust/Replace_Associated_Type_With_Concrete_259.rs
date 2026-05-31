use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Replace_Associated_Type_With_Concrete_259;

impl Mutator for Replace_Associated_Type_With_Concrete_259 {
    fn name(&self) -> &str {
        "Replace_Associated_Type_With_Concrete_259"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut candidate_types = Vec::new();
        let mut dummy_needed = false;
        for item in &file.items {
            match item {
                Item::Struct(item_struct) => {
                    candidate_types.push(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(item_struct.ident.clone()),
                    }));
                }
                Item::Enum(item_enum) => {
                    candidate_types.push(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(item_enum.ident.clone()),
                    }));
                }
                Item::Type(item_type) => {
                    candidate_types.push(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(item_type.ident.clone()),
                    }));
                }
                _ => {}
            }
        }
        if candidate_types.is_empty() {
            dummy_needed = true;
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    let trait_name = &trait_path.segments.last().unwrap().ident;
                    let mut has_generic_param = false;
                    for param in &item_impl.generics.params {
                        if matches!(param, GenericParam::Type(_)) {
                            has_generic_param = true;
                            break;
                        }
                    }
                    if has_generic_param {
                        for impl_item in &mut item_impl.items {
                            if let ImplItem::Type(assoc_type) = impl_item {
                                if assoc_type.defaultness.is_some() {
                                    let mut visitor = TypeSelfVisitor { found: false };
                                    visitor.visit_type(&assoc_type.ty);
                                    if visitor.found {
                                        let replacement_type = if dummy_needed {
                                            Type::Path(TypePath {
                                                qself: None,
                                                path: SynPath::from(Ident::new("Dummy", Span::call_site())),
                                            })
                                        } else {
                                            let mut rng = thread_rng();
                                            candidate_types.choose(&mut rng).unwrap().clone()
                                        };
                                        assoc_type.ty = replacement_type;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        if dummy_needed {
            let dummy_struct = Item::Struct(ItemStruct {
                attrs: Vec::new(),
                vis: Visibility::Public(VisPublic {
                    pub_token: token::Pub::default(),
                }),
                struct_token: token::Struct::default(),
                ident: Ident::new("Dummy", Span::call_site()),
                generics: Generics {
                    lt_token: None,
                    params: Punctuated::new(),
                    gt_token: None,
                    where_clause: None,
                },
                fields: Fields::Unit,
                semi_token: Some(token::Semi::default()),
            });
            file.items.insert(0, dummy_struct);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets trait implementations with generic parameters that have default associated types referencing Self. It replaces the Self-referential type with a concrete type from the seed program (or introduces a dummy struct if none exist). This disrupts specialization consistency, potentially causing drop glue or MIR generation bugs due to mismatched associated types."
    }
}

struct TypeSelfVisitor {
    found: bool,
}

impl Visit for TypeSelfVisitor {
    fn visit_type(&mut self, ty: &Type) {
        if let Type::Path(type_path) = ty {
            if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == "Self" {
                self.found = true;
            }
        }
        syn::visit::visit_type(self, ty);
    }
}