use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, thread_rng};
use std::default;
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, *,
};

use crate::mutator::Mutator;

pub struct Alter_And_Insert_Expr_Attributes_25;

impl Mutator for Alter_And_Insert_Expr_Attributes_25 {
    fn name(&self) -> &str {
        "Alter_And_Insert_Expr_Attributes_25"
    }

    fn mutate(&self, file: &mut syn::File) {
        struct ExprAttributeMutator;

        impl VisitMut for ExprAttributeMutator {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Lit(expr_lit) = expr {
                    let mut rng = thread_rng();
                    let chance: f64 = rng.gen();

                    if chance < 0.5 {
                        // Alter existing attribute or add a malformed one
                        if let Some(attr) = expr_lit.attrs.first_mut() {
                            attr.path().segments.last_mut().unwrap().ident = Ident::new("cfg_eval", Span::call_site());
                        } else {
                            expr_lit.attrs.push(parse_quote!(#[cfg_eval]));
                        }
                    } else {
                        // Insert a new experimental attribute
                        expr_lit.attrs.push(parse_quote!(#[cfg_eval]));
                    }
                }

                // Continue walking the AST
                visit_mut::visit_expr_mut(self, expr);
            }
        }

        let mut visitor = ExprAttributeMutator;
        visitor.visit_file_mut(file);
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator targets expressions to randomly alter existing attributes or introduce new experimental attributes. By doing so, it exposes potential parsing issues or internal compiler errors related to attribute handling. The mutator applies changes with a 50% chance to either alter or insert attributes, specifically focusing on the experimental `cfg_eval` attribute to challenge the compiler's parsing logic."
    }
}