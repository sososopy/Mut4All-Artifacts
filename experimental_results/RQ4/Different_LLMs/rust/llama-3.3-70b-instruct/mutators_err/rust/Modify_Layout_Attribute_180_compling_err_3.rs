use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    Attribute, BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
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

pub struct Modify_Layout_Attribute_180;

impl Modify_Layout_Attribute_180 {
    pub fn new() -> Self {
        Modify_Layout_Attribute_180
    }
}

impl Mutator for Modify_Layout_Attribute_180 {
    fn name(&self) -> &str {
        "Modify_Layout_Attribute_180"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for attr in &mut item_struct.attrs {
                    if let Meta::List(list) = &attr.meta {
                        if list.path.is_ident("rustc_layout_scalar_valid_range_start") {
                            let new_attr = Attribute {
                                pound_token: attr.pound_token,
                                style: AttrStyle::Inner(()),
                                bracket_token: attr.bracket_token,
                                meta: Meta::List(MetaList {
                                    path: list.path.clone(),
                                    delimiter: list.delimiter.clone(),
                                    tokens: {
                                        let mut tokens = list.tokens.clone();
                                        if let Some(NestedMeta::Meta(Meta::NameValue(name_value))) = tokens.first() {
                                            if let Lit::Int(int) = &name_value.value {
                                                let new_value = int.base10_parse::<u64>().unwrap() + 1;
                                                tokens[0] = NestedMeta::Meta(Meta::NameValue(MetaNameValue {
                                                    path: name_value.path.clone(),
                                                    eq_token: name_value.eq_token,
                                                    value: Lit::Int(LitInt::new(new_value.to_string(), int.span())),
                                                }));
                                            }
                                        }
                                        tokens
                                    },
                                }),
                            };
                            *attr = new_attr;
                        } else if list.path.is_ident("rustc_layout_scalar_valid_range_end") {
                            let new_attr = Attribute {
                                pound_token: attr.pound_token,
                                style: AttrStyle::Inner(()),
                                bracket_token: attr.bracket_token,
                                meta: Meta::List(MetaList {
                                    path: list.path.clone(),
                                    delimiter: list.delimiter.clone(),
                                    tokens: {
                                        let mut tokens = list.tokens.clone();
                                        if let Some(NestedMeta::Meta(Meta::NameValue(name_value))) = tokens.first() {
                                            if let Lit::Int(int) = &name_value.value {
                                                let new_value = int.base10_parse::<u64>().unwrap() + 1;
                                                tokens[0] = NestedMeta::Meta(Meta::NameValue(MetaNameValue {
                                                    path: name_value.path.clone(),
                                                    eq_token: name_value.eq_token,
                                                    value: Lit::Int(LitInt::new(new_value.to_string(), int.span())),
                                                }));
                                            }
                                        }
                                        tokens
                                    },
                                }),
                            };
                            *attr = new_attr;
                        }
                    }
                }
                let new_attr = Attribute {
                    pound_token: token::Pound {
                        spans: Span::call_site(),
                    },
                    style: AttrStyle::Inner(()),
                    bracket_token: token::Bracket {
                        span: Span::call_site(),
                    },
                    meta: Meta::List(MetaList {
                        path: Ident::new("rustc_layout_scalar_valid_range_end", Span::call_site()).into(),
                        delimiter: token::Comma {
                            spans: (Span::call_site(), Span::call_site()),
                        },
                        tokens: {
                            let mut elements = Punctuated::new();
                            elements.push(NestedMeta::Meta(Meta::NameValue(MetaNameValue {
                                path: Ident::new("end", Span::call_site()).into(),
                                eq_token: token::Eq {
                                    spans: (Span::call_site(), Span::call_site()),
                                },
                                value: Lit::Int(LitInt::new("1".to_string(), Span::call_site())),
                            })));
                            elements
                        },
                    }),
                };
                item_struct.attrs.push(new_attr);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator modifies the layout attribute of a struct by changing the value inside the attribute or adding a new layout attribute. This transformation helps to expose bugs related to layout attributes and their interactions with other language features."
    }
}

trait Mutator {
    fn name(&self) -> &str;
    fn mutate(&self, file: &mut syn::File);
    fn chain_of_thought(&self) -> &str;
}