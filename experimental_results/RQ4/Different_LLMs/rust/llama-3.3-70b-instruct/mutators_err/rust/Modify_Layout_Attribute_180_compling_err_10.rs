use proc_macro2::{Span, proc_macro2::extra::DelimSpan, *};
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
                                style: AttrStyle::Inner(token::Not {
                                    spans: [Span::call_site()],
                                }),
                                bracket_token: attr.bracket_token,
                                meta: Meta::List(MetaList {
                                    path: list.path.clone(),
                                    delimiter: token::Comma {
                                        spans: [Span::call_site()],
                                    }.into(),
                                    tokens: {
                                        let mut tokens = list.tokens.clone();
                                        if let Some(TokenTree::Literal(lit)) = tokens.into_iter().next() {
                                            if let Literal::Int(int) = lit {
                                                let new_value = int.base10_parse::<u64>().unwrap() + 1;
                                                let new_lit = Literal::u64_suffixed(new_value);
                                                let new_tokens = TokenStream::from(TokenTree::Literal(new_lit));
                                                tokens = new_tokens;
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
                                style: AttrStyle::Inner(token::Not {
                                    spans: [Span::call_site()],
                                }),
                                bracket_token: attr.bracket_token,
                                meta: Meta::List(MetaList {
                                    path: list.path.clone(),
                                    delimiter: token::Comma {
                                        spans: [Span::call_site()],
                                    }.into(),
                                    tokens: {
                                        let mut tokens = list.tokens.clone();
                                        if let Some(TokenTree::Literal(lit)) = tokens.into_iter().next() {
                                            if let Literal::Int(int) = lit {
                                                let new_value = int.base10_parse::<u64>().unwrap() + 1;
                                                let new_lit = Literal::u64_suffixed(new_value);
                                                let new_tokens = TokenStream::from(TokenTree::Literal(new_lit));
                                                tokens = new_tokens;
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
                        spans: [Span::call_site()],
                    },
                    style: AttrStyle::Inner(token::Not {
                        spans: [Span::call_site()],
                    }),
                    bracket_token: token::Bracket {
                        span: DelimSpan::from(Span::call_site()),
                    },
                    meta: Meta::List(MetaList {
                        path: Ident::new("rustc_layout_scalar_valid_range_end", Span::call_site()).into(),
                        delimiter: token::Comma {
                            spans: [Span::call_site()],
                        }.into(),
                        tokens: {
                            let mut elements = Punctuated::new();
                            elements.push(Expr::Lit(ExprLit {
                                attrs: vec![],
                                lit: Lit::Int(LitInt::new("1", Span::call_site())),
                            }));
                            elements.into_token_stream()
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