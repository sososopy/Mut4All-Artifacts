use crate::mutator::Mutator;
use syn::{parse_quote, Item, ItemStruct, GenericParam, Expr, ExprBlock, Stmt, ExprParen, token::Semi};

pub struct Modify_Const_Generic_Default_52;

impl Mutator for Modify_Const_Generic_Default_52 {
    fn name(&self) -> &str {
        "Modify_Const_Generic_Default_52"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(ItemStruct { generics, .. }) = item {
                for param in &mut generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some(default) = &mut const_param.default {
                            if let Expr::Paren(ExprParen { expr, .. }) = default.as_mut() {
                                if let Expr::Block(expr_block) = expr.as_mut() {
                                    let new_expr: Expr = parse_quote!({ 10 / 0 });
                                    *expr = Expr::Block(ExprBlock {
                                        attrs: expr_block.attrs.clone(),
                                        label: None,
                                        block: syn::Block {
                                            brace_token: expr_block.block.brace_token,
                                            stmts: vec![Stmt::Expr(new_expr)],
                                        },
                                    });
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct definitions with constant generic parameters having default values. It modifies the default value by replacing the existing constant expression with a potentially problematic one, such as a division by zero. This aims to reveal issues in the compiler's handling of const generics by introducing expressions that may cause internal compiler errors or other unexpected behaviors."
    }
}