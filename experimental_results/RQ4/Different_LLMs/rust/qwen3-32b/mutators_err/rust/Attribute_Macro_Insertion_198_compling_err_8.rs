use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, prelude::IteratorRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    BoundLifetimes, Expr, ExprMacro, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, StmtMacro, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
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
use syn::NestedMeta;

use crate::mutator::Mutator;

pub struct Attribute_Macro_Insertion_198;

struct LintCollector {
    lint_names: HashSet<String>,
}

impl LintCollector {
    fn new() -> Self {
        Self {
            lint_names: HashSet::new(),
        }
    }
}

impl<'ast> Visit<'ast> for LintCollector {
    fn visit_attribute(&mut self, attr: &syn::Attribute) {
        if attr.path().is_ident("warn") || attr.path().is_ident("allow") {
            let meta = &attr.meta;
            if let syn::Meta::List(list) = meta {
                for nested in list.nested() {
                    if let NestedMeta::Meta(meta_item) = nested {
                        if let syn::Meta::Path(path) = meta_item {
                            let mut lint_name = String::new();
                            for (i, segment) in path.segments.iter().enumerate() {
                                if i > 0 {
                                    lint_name.push_str("::");
                                }
                                lint_name.push_str(&segment.ident.to_string());
                            }
                            self.lint_names.insert(lint_name);
                        }
                    }
                }
            }
        }
    }
}

struct MacroMutator {
    lint_names: HashSet<String>,
}

impl MacroMutator {
    fn choose_lint(&self) -> (String, String) {
        let mut rng = thread_rng();
        let use_expect = rng.r#gen();
        let attr_name = if use_expect { "expect".to_string() } else { "allow".to_string() };
        let lint_name = if !self.lint_names.is_empty() {
            self.lint_names.iter().choose(&mut rng).unwrap().clone()
        } else {
            "unreachable_code".to_string()
        };
        (attr_name, lint_name)
    }
}

impl VisitMut for MacroMutator {
    fn visit_expr_mut(&mut self, expr: &mut Expr) {
        if let Expr::Macro(expr_macro) = expr {
            let (attr_name, lint_name) = self.choose_lint();
            let attr = parse_quote!(#attr_name(#lint_name));
            expr_macro.attrs.push(attr);
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }

    fn visit_stmt_mut(&mut self, stmt: &mut Stmt) {
        if let Stmt::Macro(stmt_macro) = stmt {
            let (attr_name, lint_name) = self.choose_lint();
            let attr = parse_quote!(#attr_name(#lint_name));
            stmt_macro.attrs.push(attr);
        }
        syn::visit_mut::visit_stmt_mut(self, stmt);
    }
}

impl Mutator for Attribute_Macro_Insertion_198 {
    fn name(&self) -> &str {
        "Attribute_Macro_Insertion_198"
    }

    fn mutate(&self, file: &mut File) {
        let mut lint_collector = LintCollector::new();
        lint_collector.visit_file(file);
        let lint_names = lint_collector.lint_names;

        let mut macro_mutator = MacroMutator { lint_names };
        macro_mutator.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts an #[expect] or #[allow] attribute with a lint name from the seed program's diagnostics onto macro invocations in expression or statement contexts where attribute propagation is not supported. This forces the compiler to process diagnostic levels during macro expansion, potentially exposing inconsistencies in how attributes are applied to macro-generated code."
    }
}