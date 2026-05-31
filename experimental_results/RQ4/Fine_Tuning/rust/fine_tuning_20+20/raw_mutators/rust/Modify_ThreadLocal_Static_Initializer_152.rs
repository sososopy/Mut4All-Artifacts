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

pub struct Modify_ThreadLocal_Static_Initializer_152;

impl Mutator for Modify_ThreadLocal_Static_Initializer_152 {
    fn name(&self) -> &str {
        "Modify_ThreadLocal_Static_Initializer_152"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                let mut new_items = Vec::new();
                for impl_item in &item_impl.items {
                    if let ImplItem::Const(item_const) = impl_item {
                        if let Expr::Macro(expr_macro) = &item_const.expr {
                            if expr_macro.mac.path.is_ident("thread_local") {
                                let mut new_tokens = proc_macro2::TokenStream::new();
                                let mut inside_braces = false;
                                let mut brace_depth = 0;
                                let mut inside_initializer = false;
                                let mut initializer_tokens =
                                    proc_macro2::TokenStream::new();
                                for token in expr_macro.mac.tokens.clone() {
                                    match token {
                                        proc_macro2::TokenTree::Group(group) => {
                                            if group.delimiter()
                                                == proc_macro2::Delimiter::Brace
                                            {
                                                if inside_braces {
                                                    brace_depth += 1;
                                                } else {
                                                    inside_braces = true;
                                                }
                                            }
                                            if inside_braces {
                                                if inside_initializer {
                                                    initializer_tokens
                                                        .extend(Some(
                                                            proc_macro2::TokenTree::Group(
                                                                proc_macro2::Group::new(
                                                                    group.delimiter(),
                                                                    group.stream(),
                                                                ),
                                                            ),
                                                        ));
                                                } else {
                                                    new_tokens.extend(Some(
                                                        proc_macro2::TokenTree::Group(
                                                            proc_macro2::Group::new(
                                                                group.delimiter(),
                                                                group.stream(),
                                                            ),
                                                        ),
                                                    ));
                                                }
                                            }
                                        }
                                        proc_macro2::TokenTree::Ident(ident) => {
                                            if inside_braces {
                                                if ident == "const" {
                                                    inside_initializer = true;
                                                }
                                                if inside_initializer {
                                                    initializer_tokens
                                                        .extend(Some(
                                                            proc_macro2::TokenTree::Ident(
                                                                ident,
                                                            ),
                                                        ));
                                                } else {
                                                    new_tokens.extend(Some(
                                                        proc_macro2::TokenTree::Ident(
                                                            ident,
                                                        ),
                                                    ));
                                                }
                                            }
                                        }
                                        proc_macro2::TokenTree::Punct(punct) => {
                                            if inside_braces {
                                                if punct.as_char() == '{' {
                                                    brace_depth += 1;
                                                } else if punct.as_char() == '}' {
                                                    if brace_depth > 0 {
                                                        brace_depth -= 1;
                                                    } else {
                                                        inside_braces = false;
                                                    }
                                                }
                                                if inside_initializer {
                                                    initializer_tokens
                                                        .extend(Some(
                                                            proc_macro2::TokenTree::Punct(
                                                                punct,
                                                            ),
                                                        ));
                                                } else {
                                                    new_tokens.extend(Some(
                                                        proc_macro2::TokenTree::Punct(
                                                            punct,
                                                        ),
                                                    ));
                                                }
                                            }
                                        }
                                        proc_macro2::TokenTree::Literal(literal) => {
                                            if inside_braces {
                                                if inside_initializer {
                                                    initializer_tokens
                                                        .extend(Some(
                                                            proc_macro2::TokenTree::Literal(
                                                                literal,
                                                            ),
                                                        ));
                                                } else {
                                                    new_tokens.extend(Some(
                                                        proc_macro2::TokenTree::Literal(
                                                            literal,
                                                        ),
                                                    ));
                                                }
                                            }
                                        }
                                    }
                                }
                                if !initializer_tokens.is_empty() {
                                    let new_initializer: Expr = parse_quote!({
                                        |inner| {
                                            #initializer_tokens;
                                            inner
                                        }
                                    });
                                    new_tokens.extend(quote!(#new_initializer));
                                }
                                let new_expr = Expr::Macro(syn::ExprMacro {
                                    attrs: expr_macro.attrs.clone(),
                                    mac: syn::Macro {
                                        path: expr_macro.mac.path.clone(),
                                        bang_token: expr_macro.mac.bang_token,
                                        delimiter: expr_macro.mac.delimiter,
                                        tokens: new_tokens,
                                    },
                                });
                                new_items.push(ImplItem::Const(ImplItemConst {
                                    attrs: item_const.attrs.clone(),
                                    vis: item_const.vis.clone(),
                                    defaultness: item_const.defaultness,
                                    const_token: item_const.const_token,
                                    ident: item_const.ident.clone(),
                                    generics: item_const.generics.clone(),
                                    colon_token: item_const.colon_token,
                                    ty: item_const.ty.clone(),
                                    expr: Box::new(new_expr),
                                    semi_token: item_const.semi_token,
                                }));
                            }
                        }
                    } else {
                        new_items.push(impl_item.clone());
                    }
                }
                *item = Item::Impl(ItemImpl {
                    attrs: item_impl.attrs.clone(),
                    defaultness: item_impl.defaultness,
                    unsafety: item_impl.unsafety,
                    impl_token: item_impl.impl_token,
                    generics: item_impl.generics.clone(),
                    trait_: item_impl.trait_.clone(),
                    self_ty: item_impl.self_ty.clone(),
                    brace_token: item_impl.brace_token,
                    items: new_items,
                });
            }
            if let Item::Struct(item_struct) = item {
                let mut new_fields = Vec::new();
                for field in &item_struct.fields {
                    if let Some((_, expr)) = &field.expr {
                        if let Expr::Macro(expr_macro) = expr {
                            if expr_macro.mac.path.is_ident("thread_local") {
                                let mut new_tokens = proc_macro2::TokenStream::new();
                                let mut inside_braces = false;
                                let mut brace_depth = 0;
                                let mut inside_initializer = false;
                                let mut initializer_tokens =
                                    proc_macro2::TokenStream::new();
                                for token in expr_macro.mac.tokens.clone() {
                                    match token {
                                        proc_macro2::TokenTree::Group(group) => {
                                            if group.delimiter()
                                                == proc_macro2::Delimiter::Brace
                                            {
                                                if inside_braces {
                                                    brace_depth += 1;
                                                } else {
                                                    inside_braces = true;
                                                }
                                            }
                                            if inside_braces {
                                                if inside_initializer {
                                                    initializer_tokens
                                                        .extend(Some(
                                                            proc_macro2::TokenTree::Group(
                                                                proc_macro2::Group::new(
                                                                    group.delimiter(),
                                                                    group.stream(),
                                                                ),
                                                            ),
                                                        ));
                                                } else {
                                                    new_tokens.extend(Some(
                                                        proc_macro2::TokenTree::Group(
                                                            proc_macro2::Group::new(
                                                                group.delimiter(),
                                                                group.stream(),
                                                            ),
                                                        ),
                                                    ));
                                                }
                                            }
                                        }
                                        proc_macro2::TokenTree::Ident(ident) => {
                                            if inside_braces {
                                                if ident == "const" {
                                                    inside_initializer = true;
                                                }
                                                if inside_initializer {
                                                    initializer_tokens
                                                        .extend(Some(
                                                            proc_macro2::TokenTree::Ident(
                                                                ident,
                                                            ),
                                                        ));
                                                } else {
                                                    new_tokens.extend(Some(
                                                        proc_macro2::TokenTree::Ident(
                                                            ident,
                                                        ),
                                                    ));
                                                }
                                            }
                                        }
                                        proc_macro2::TokenTree::Punct(punct) => {
                                            if inside_braces {
                                                if punct.as_char() == '{' {
                                                    brace_depth += 1;
                                                } else if punct.as_char() == '}' {
                                                    if brace_depth > 0 {
                                                        brace_depth -= 1;
                                                    } else {
                                                        inside_braces = false;
                                                    }
                                                }
                                                if inside_initializer {
                                                    initializer_tokens
                                                        .extend(Some(
                                                            proc_macro2::TokenTree::Punct(
                                                                punct,
                                                            ),
                                                        ));
                                                } else {
                                                    new_tokens.extend(Some(
                                                        proc_macro2::TokenTree::Punct(
                                                            punct,
                                                        ),
                                                    ));
                                                }
                                            }
                                        }
                                        proc_macro2::TokenTree::Literal(literal) => {
                                            if inside_braces {
                                                if inside_initializer {
                                                    initializer_tokens
                                                        .extend(Some(
                                                            proc_macro2::TokenTree::Literal(
                                                                literal,
                                                            ),
                                                        ));
                                                } else {
                                                    new_tokens.extend(Some(
                                                        proc_macro2::TokenTree::Literal(
                                                            literal,
                                                        ),
                                                    ));
                                                }
                                            }
                                        }
                                    }
                                }
                                if !initializer_tokens.is_empty() {
                                    let new_initializer: Expr = parse_quote!({
                                        |inner| {
                                            #initializer_tokens;
                                            inner
                                        }
                                    });
                                    new_tokens.extend(quote!(#new_initializer));
                                }
                                let new_expr = Expr::Macro(syn::ExprMacro {
                                    attrs: expr_macro.attrs.clone(),
                                    mac: syn::Macro {
                                        path: expr_macro.mac.path.clone(),
                                        bang_token: expr_macro.mac.bang_token,
                                        delimiter: expr_macro.mac.delimiter,
                                        tokens: new_tokens,
                                    },
                                });
                                new_fields.push(Field {
                                    attrs: field.attrs.clone(),
                                    member: field.member.clone(),
                                    colon_token: field.colon_token,
                                    expr: Some((
                                        field.expr.as_ref().unwrap().0.clone(),
                                        new_expr,
                                    )),
                                });
                            }
                        }
                    } else {
                        new_fields.push(field.clone());
                    }
                }
                *item = Item::Struct(ItemStruct {
                    attrs: item_struct.attrs.clone(),
                    struct_token: item_struct.struct_token,
                    ident: item_struct.ident.clone(),
                    fields: new_fields,
                    semi_token: item_struct.semi_token,
                });
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets thread-local static initializers within structs and impls, specifically those using the `thread_local!` macro. It modifies the initializer by wrapping it in a closure that captures an inner closure, enhancing the complexity of the initialization logic. This transformation stresses the compiler's ability to handle nested closures and thread-local statics, potentially revealing issues in macro expansion, closure capture analysis, and thread-local variable management."
    }
}