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

pub struct MethodCallWithColonInInvalidPath_232;

impl Mutator for MethodCallWithColonInInvalidPath_232 {
    fn name(&self) -> &str {
        "MethodCallWithColonInInvalidPath_232"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = CallMutator {};
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets method calls on path expressions (e.g., `Type::method(...)`) and transforms them into invalid syntax by replacing the method name with a parenthesized identifier followed by a colon and a token from the seed program (e.g., `Type::(x: !)(...)`). This mimics an invalid colon in a path segment, potentially exposing parser ambiguities or ICEs in the compiler's handling of method call syntax."
    }
}

struct CallMutator {
    identifiers: HashSet<Ident>,
    tokens: Vec<String>,
}

impl CallMutator {
    fn should_mutate(&self) -> bool {
        thread_rng().gen_bool(0.3)
    }

    fn get_random_identifier(&self) -> Option<&Ident> {
        if self.identifiers.is_empty() {
            return None;
        }
        let mut rng = thread_rng();
        let index = rng.gen_range(0..self.identifiers.len());
        self.identifiers.iter().nth(index)
    }

    fn get_random_token(&self) -> Option<&str> {
        if self.tokens.is_empty() {
            return None;
        }
        let mut rng = thread_rng();
        let index = rng.gen_range(0..self.tokens.len());
        self.tokens.get(index)
    }
}

impl VisitMut for CallMutator {
    fn visit_expr_call_mut(&mut self, call: &mut ExprCall) {
        if let Expr::Path(expr_path) = &call.func {
            let path = &expr_path.path;
            if path.segments.len() == 1 {
                // This is a method call on a path like `Type::method(...)`
                if self.should_mutate() {
                    // Collect identifiers and tokens from the file to use in mutation
                    let mut visitor = IdentCollector {};
                    visitor.visit_file(path);
                    self.identifiers = visitor.identifiers;
                    let token_visitor = TokenCollector {};
                    let source = match path.span().source_text() {
                        Ok(s) => s,
                        Err(_) => return,
                    };
                    self.tokens = token_visitor.collect_tokens(&source);

                    if let Some(id) = self.get_random_identifier() {
                        if let Some(token_str) = self.get_random_token() {
                            let new_method = parse_quote! {
                                (#id: #token_str)
                            };
                            let new_call = parse_quote! {
                                #path::#new_method(#call.args)
                            };
                            *call = new_call;
                        }
                    }
                }
            }
        }
        visit_mut::visit_expr_call_mut(self, call);
    }
}

struct IdentCollector {
    identifiers: HashSet<Ident>,
}

impl Visit for IdentCollector {
    fn visit_ident(&mut self, ident: &Ident) {
        self.identifiers.insert(ident.clone());
    }
}

struct TokenCollector {}

impl TokenCollector {
    fn collect_tokens(&self, source: &str) -> Vec<String> {
        let re = Regex::new(r#"[!@#$%^&*()+=\{\}\[\]:;"',<>.?/|\\~`-]"#).unwrap();
        re.find_iter(source)
            .map(|m| source[m.start()..m.end()].to_string())
            .collect()
    }
}