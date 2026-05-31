use proc_macro2::{Span, TokenStream, TokenTree};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{
    collections::HashSet,
    default,
    fs,
    ops::Range,
    panic,
    path::Path,
    process::Command,
    str::FromStr,
    *,
};
use syn::{
    parse::{Parse, ParseStream},
    parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

struct MacroRules {
    rules: Vec<MacroRule>,
}

impl Parse for MacroRules {
    fn parse(input: ParseStream) -> Result<Self> {
        let mut rules = Vec::new();
        while !input.is_empty() {
            rules.push(input.parse()?);
            if input.is_empty() {
                break;
            }
            input.parse::<Token![;]>()?;
        }
        Ok(MacroRules { rules })
    }
}

struct MacroRule {
    pattern: String,
    body: String,
}

impl Parse for MacroRule {
    fn parse(input: ParseStream) -> Result<Self> {
        let pattern;
        let body;
        let _ = syn::braced!(pattern in input);
        let _ = syn::braced!(body in input);
        Ok(MacroRule {
            pattern: pattern.span().source_text().unwrap_or_default(),
            body: body.span().source_text().unwrap_or_default(),
        })
    }
}

use crate::mutator::Mutator;

pub struct Replace_Macro_Var_Decl_With_Invalid_Keyword_228;

impl Mutator for Replace_Macro_Var_Decl_With_Invalid_Keyword_228 {
    fn name(&self) -> &str {
        "Replace_Macro_Var_Decl_With_Invalid_Keyword_228"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MacroVisitor {
            targets: Vec<Ident>,
        }

        impl<'ast> Visit<'ast> for MacroVisitor {
            fn visit_item_macro(&mut self, node: &'ast ItemMacro) {
                if let Some((_, ref mac)) = node.mac {
                    if mac.path.is_ident("macro_rules") {
                        if let Ok(parsed) = syn::parse2::<MacroRules>(mac.tokens.clone()) {
                            for rule in parsed.rules {
                                if rule.pattern.contains("$s:stmt")
                                    || rule.pattern.contains("$e:expr")
                                {
                                    self.targets.push(node.mac.path.segments[0].ident.clone());
                                }
                            }
                        }
                    }
                }
                visit::visit_item_macro(self, node);
            }
        }

        struct MacroCallVisitor {
            targets: HashSet<String>,
            calls: Vec<ExprMacro>,
        }

        impl<'ast> Visit<'ast> for MacroCallVisitor {
            fn visit_expr_macro(&mut self, node: &'ast ExprMacro) {
                if self.targets.contains(&node.mac.path.segments[0].ident.to_string()) {
                    self.calls.push(node.clone());
                }
                visit::visit_expr_macro(self, node);
            }
        }

        let mut visitor = MacroVisitor { targets: vec![] };
        visitor.visit_file(file);
        let targets: HashSet<String> = visitor.targets.iter().map(|id| id.to_string()).collect();

        let mut call_visitor = MacroCallVisitor {
            targets,
            calls: vec![],
        };
        call_visitor.visit_file(file);

        let mut rng = thread_rng();
        let mut selected = call_visitor.calls.clone();
        selected.shuffle(&mut rng);

        for expr_mac in selected {
            let mut modified = false;
            let mut new_tokens = TokenStream::new();
            let mut inside_var_decl = false;
            let mut var_decl_tokens = TokenStream::new();
            for tt in expr_mac.mac.tokens {
                match &tt {
                    proc_macro2::TokenTree::Ident(ident) => {
                        if ident == "let" || ident == "const" {
                            inside_var_decl = true;
                            var_decl_tokens.extend(Some(tt));
                        } else if inside_var_decl {
                            var_decl_tokens.extend(Some(tt));
                        } else {
                            new_tokens.extend(Some(tt));
                        }
                    }
                    proc_macro2::TokenTree::Punct(punct) => {
                        if inside_var_decl {
                            var_decl_tokens.extend(Some(tt.clone()));
                            if punct.as_char() == ';' {
                                inside_var_decl = false;
                                let var_decl_str = var_decl_tokens.to_string();
                                let modified_decl =
                                    var_decl_str.replacen("let", "var", 1).replacen("const", "var", 1);
                                new_tokens.extend(TokenStream::from_str(&modified_decl).ok());
                                var_decl_tokens = TokenStream::new();
                                modified = true;
                            }
                        } else {
                            new_tokens.extend(Some(tt));
                        }
                    }
                    _ => {
                        if inside_var_decl {
                            var_decl_tokens.extend(Some(tt));
                        } else {
                            new_tokens.extend(Some(tt));
                        }
                    }
                }
            }
            if modified {
                expr_mac.mac.tokens = new_tokens;
                break;
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutator targets macro invocations that include variable declarations by replacing the `let` or `const` keyword with an invalid `var` keyword. It first identifies macros defined with `macro_rules!` that accept statements or expressions, then locates their invocations. Within these macro calls, it transforms valid variable declarations into syntactically incorrect ones, aiming to introduce semantic errors that stress the compiler's macro expansion and parsing logic."
    }
}