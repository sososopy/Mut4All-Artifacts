//source file
#include "../include/modify_pointer_dereference_in_constant_expressions_8.h"

// ========================================================================================================
#define MUT8_OUTPUT 1

void MutatorFrontendAction_8::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("constVar")) {
        //Filter nodes in header files
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       VD->getLocation()))
            return;

        if (VD->isConstexpr() && VD->hasInit()) {
            if (auto *UO = dyn_cast<UnaryOperator>(VD->getInit())) {
                if (UO->getOpcode() == UO_Deref) {
                    //Get the source code text of target node
                    auto initExpr = UO->getSubExpr();
                    if (auto *IL = dyn_cast<IntegerLiteral>(initExpr)) {
                        if (IL->getValue().isZero()) {
                            //Perform mutation on the source code text by applying string replacement
                            std::string newVarDecl = "long validValue = 42;";
                            std::string newInit = "&validValue";
                            SourceLocation startLoc = VD->getBeginLoc();
                            Rewrite.InsertTextBefore(startLoc, newVarDecl + "\n");
                            Rewrite.ReplaceText(UO->getSourceRange(), newInit);
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_8::MutatorASTConsumer_8::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isConstexpr(), hasInitializer(unaryOperator(hasOperatorName("*"), hasUnaryOperand(integerLiteral(equals(0)))))).bind("constVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}