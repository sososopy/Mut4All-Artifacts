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

pub struct Attribute_Macro_Expansion_Emptiness_28;

impl Mutator for Attribute_Macro_Expansion_Emptiness_28 {
    fn name(&self) -> &str {
        "Attribute_Macro_Expansion_Emptiness_28"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut has_empty_macro = false;
        for item in &mut file.items {
            if let Item::Macro(item_macro) = item {
                if item_macro.mac.path.is_ident("empty_macro") {
                    has_empty_macro = true;
                }
            }
        }
        if !has_empty_macro {
            let empty_macro_def = parse_quote! {
                macro_rules! empty_macro { () => {}; }
            };
            file.items.insert(0, empty_macro_def);
        }
        let mut visitor = AttributeMacroVisitor {
            empty_macro_ident: Ident::new("empty_macro", Span::call_site()),
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets attribute arguments that are macro invocations. It replaces the macro invocation inside an attribute with a call to `empty_macro`, which expands to an empty token stream. This mimics bug-triggering conditions where attribute parsers receive no content from macros, potentially exposing missing checks for empty expansions. It ensures `empty_macro` is defined if not already present, then traverses the AST to replace macro calls in attribute arguments."
    }
}

struct AttributeMacroVisitor {
    empty_macro_ident: Ident,
}

impl VisitMut for AttributeMacroVisitor {
    fn visit_attribute_mut(&mut self, attr: &mut Attribute) {
        match attr.meta {
            Meta::List(list) => {
                let mut tokens = list.tokens.clone().into_iter().collect::<Vec<_>>();
                for i in 0..tokens.len() {
                    if let proc_macro2::TokenTree::Ident(ident) = &tokens[i] {
                        if i + 1 < tokens.len() {
                            if let proc_macro2::TokenTree::Punct(punct) = &tokens[i + 1] {
                                if punct.as_char() == '!' {
                                    let new_macro = proc_macro2::TokenTree::Ident(self.empty_macro_ident.clone());
                                    tokens[i] = new_macro;
                                }
                            }
                        }
                    }
                }
                let new_tokens = tokens.into_iter().collect();
                attr.meta = Meta::List(MetaList {
                    path: list.path.clone(),
                    delimiter: list.delimiter.clone(),
                    tokens: new_tokens,
                });
            }
            Meta::NameValue(name_value) => {
                if let Expr::Macro(expr_macro) = &name_value.value {
                    let new_macro = Expr::Macro(ExprMacro {
                        attrs: expr_macro.attrs.clone(),
                        mac: Macro {
                            path: SynPath::from(self.empty_macro_ident.clone()),
                            bang_token: expr_macro.mac.bang_token,
                            delimiter: expr_macro.mac.delimiter.clone(),
                            tokens: expr_macro.mac.tokens.clone(),
                        },
                    });
                    attr.meta = Meta::NameValue(MetaNameValue {
                        path: name_value.path.clone(),
                        eq_token: name_value.eq_token,
                        value: new_macro,
                    });
                }
            }
            _ => {}
        }
        syn::visit_mut::visit_attribute_mut(self, attr);
    }
}