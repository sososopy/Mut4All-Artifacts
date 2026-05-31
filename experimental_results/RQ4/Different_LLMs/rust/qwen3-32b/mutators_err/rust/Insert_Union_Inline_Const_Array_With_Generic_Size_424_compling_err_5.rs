use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemUnion, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeArray, TypeImplTrait,
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

pub struct Insert_Union_Inline_Const_Array_With_Generic_Size_424;

impl Mutator for Insert_Union_Inline_Const_Array_With_Generic_Size_424 {
    fn name(&self) -> &str {
        "Insert_Union_Inline_Const_Array_With_Generic_Size_424"
    }
    fn mutate(&self, file: &mut syn::File) {
        let has_mem = file.items.iter().any(|item| {
            if let Item::Use(use_item) = item {
                let mut path_segments = Vec::new();
                let mut current = &use_item.tree;
                loop {
                    match current {
                        UseTree::Path(path) => {
                            if let UseTree::Name(name) = &*path.tree {
                                path_segments.push(name.ident.to_string());
                                current = &path.tree;
                            } else {
                                break;
                            }
                        },
                        UseTree::Name(name) => {
                            path_segments.push(name.ident.to_string());
                            break;
                        },
                        _ => break,
                    }
                }
                if path_segments.len() == 2 && path_segments[0] == "std" && path_segments[1] == "mem" {
                    true
                } else {
                    false
                }
            } else {
                false
            }
        });

        if !has_mem {
            let use_mem = parse_quote! {
                use std::mem;
            };
            file.items.insert(0, Item::Use(use_mem));
        }

        for item in &mut file.items {
            if let Item::Union(union) = item {
                if union.generics.params.is_empty() {
                    let new_param = parse_quote!(T);
                    union.generics.params.push(new_param);
                }

                let first_param = union.generics.params.first().unwrap();
                let param_name = if let GenericParam::Type(type_param) = first_param {
                    &type_param.ident
                } else {
                    panic!("Expected a type parameter");
                };

                let size_of_path = SynPath {
                    leading_colon: None,
                    segments: {
                        let mut segments = Punctuated::new();
                        segments.push(PathSegment::from(Ident::new("std", Span::call_site())));
                        segments.push(PathSegment::from(Ident::new("mem", Span::call_site())));
                        segments.push(PathSegment {
                            ident: Ident::new("size_of", Span::call_site()),
                            arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
                                colon2_token: None,
                                lt_token: token::Lt::default(),
                                args: Punctuated::from_iter(vec![GenericArgument::Type(
                                    Type::Path(TypePath {
                                        qself: None,
                                        path: SynPath::from(param_name.clone()),
                                    }),
                                )]),
                                gt_token: token::Gt::default(),
                            }),
                        });
                        segments
                    },
                };

                let call_expr = ExprCall {
                    attrs: vec![],
                    func: Box::new(Expr::Path(ExprPath {
                        attrs: vec![],
                        qself: None,
                        path: size_of_path,
                    })),
                    paren_token: Paren::default(),
                    args: Punctuated::new(),
                };

                let block = syn::Block {
                    stmts: vec![Stmt::Expr(call_expr, None)],
                    brace_token: token::Brace::default(),
                };

                let const_expr = Expr::Const(ExprConst {
                    attrs: vec![],
                    const_token: token::Const::default(),
                    block: Box::new(block),
                });

                let array_type = Type::Array(TypeArray {
                    bracket_token: token::Bracket::default(),
                    elem: Box::new(Type::Path(TypePath {
                        qself: None,
                        path: SynPath::from(Ident::new("u8", Span::call_site())),
                    })),
                    len: const_expr,
                    semi_token: token::Semi::default(),
                });

                let new_field = Field {
                    attrs: vec![],
                    vis: Visibility::Inherited,
                    ident: Some(Ident::new("as_bytes", Span::call_site())),
                    colon_token: Some(token::Colon::default()),
                    ty: array_type,
                    mutability: FieldMutability::None,
                };

                if let syn::Fields::Named(fields) = &mut union.fields {
                    fields.named.push(new_field);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}