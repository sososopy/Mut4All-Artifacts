use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Rename_Macro_Identifiers_Ensure_Literal_Formatting_204;

impl Mutator for Rename_Macro_Identifiers_Ensure_Literal_Formatting_204 {
    fn name(&self) -> &str {
        "Rename_Macro_Identifiers_Ensure_Literal_Formatting_204"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Macro(mac) = item {
                if let Some(mac_rules) = mac.mac.path.segments.first() {
                    if mac_rules.ident == "macro_rules" {
                        let mut tokens = mac.mac.tokens.clone();
                        let re = Regex::new(r"\$(crate|ty|fmt)::([a-zA-Z_][a-zA-Z0-9_]*)").unwrap();
                        let mut new_tokens = TokenStream::new();

                        for token in tokens {
                            match token {
                                TokenTree::Ident(ident) => {
                                    let ident_str = ident.to_string();
                                    if re.is_match(&ident_str) {
                                        let new_ident_str = re.replace_all(&ident_str, |caps: &regex::Captures| {
                                            format!("${}::{}", &caps[1], &caps[2])
                                        });
                                        new_tokens.extend(quote! { #new_ident_str });
                                    } else {
                                        new_tokens.extend(quote! { #ident });
                                    }
                                }
                                TokenTree::Literal(lit) => {
                                    let lit_str = lit.to_string();
                                    if lit_str.starts_with("$") && !lit_str.starts_with("\"") {
                                        new_tokens.extend(quote! { #lit_str });
                                    } else {
                                        new_tokens.extend(quote! { #lit });
                                    }
                                }
                                _ => new_tokens.extend(quote! { #token }),
                            }
                        }
                        mac.mac.tokens = new_tokens;
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macros defined with `macro_rules!`, focusing on the identifiers used within them. It ensures proper formatting of identifiers like `$crate`, `$ty`, and `$fmt`, correcting any misspelled or unknown prefixes. Additionally, it checks literals that resemble prefixes to ensure they are properly quoted. This transformation aims to prevent internal compiler errors (ICEs) by enforcing consistent identifier and literal formatting in macro definitions."
    }
}