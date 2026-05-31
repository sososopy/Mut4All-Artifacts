//source file
#include "../include/conditional_expression_with_struct_member_access_21.h"

// ========================================================================================================
#define MUT21_OUTPUT 1

void MutatorFrontendAction_21::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        auto macroLoc = FD->getLocation(); // Use function location as a placeholder for missing MacroDirective
        auto macroText = Result.Context->getSourceManager().getCharacterData(macroLoc);

        if (FD->getNumParams() > 0) {
            if (auto *PVD = dyn_cast<ParmVarDecl>(FD->getParamDecl(0))) {
                if (auto *RT = dyn_cast<clang::PointerType>(PVD->getType().getTypePtr())) {
                    if (auto *RD = dyn_cast<clang::RecordType>(RT->getPointeeType().getTypePtr())) {
                        if (RD->getDecl()->isStruct()) {
                            auto structName = RD->getDecl()->getNameAsString();
                            auto memberName = "member"; // Assuming member is known or can be retrieved
                            std::string mutation = "constexpr int result = " + std::string(macroText) + "(10, " + PVD->getNameAsString() + "->" + memberName + ");";
                            Rewrite.InsertTextAfterToken(FD->getBody()->getBeginLoc(), "\n/*mut21*/" + mutation + "\n");
                        }
                    }
                }
            }
        }
    }
}
  
void MutatorFrontendAction_21::MutatorASTConsumer_21::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto functionMatcher = functionDecl(hasBody(compoundStmt())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}