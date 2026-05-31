use syn::{parse_quote, Item, Expr, Stmt};
use crate::mutator::Mutator;

struct Insert_Drop_In_Static_Block_128;

impl Mutator for Insert_Drop_In_Static_Block_128 {
    fn name(&self) -> &str {
        "Insert_Drop_In_Static_Block_128"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Static(static_item) = item {
                let mut new_exprs = Vec::new();
                if let Expr::Block(block) = &*static_item.expr {
                    for stmt in &block.block.stmts {
                        new_exprs.push(stmt.clone());
                        if let Stmt::Local(local) = stmt {
                            if let Some(init) = &local.init {
                                if let Expr::Array(array) = init {
                                    let drop_stmt = Stmt::Expr(
                                        parse_quote! {
                                            drop(#local.pat);
                                        },
                                        None,
                                    );
                                    new_exprs.push(drop_stmt);
                                }
                            }
                        }
                    }
                    static_item.expr = parse_quote! {
                        {
                            #(#new_exprs)*
                        }
                    };
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts a drop statement for variables in static blocks, especially when the variable is an array or collection. This transformation aims to test the compiler's handling of drop order and const evaluation, potentially exposing bugs related to destructor calls and static initialization."
    }
}