//source file
#include "../include/specialize_template_with_static_assert_617.h"

// ========================================================================================================
#define MUT617_OUTPUT 1

void MutatorFrontendAction_617::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FT->getLocation()))
            return;

        if (FT->getTemplatedDecl() && FT->getTemplatedDecl()->hasBody()) {
            auto templateName = FT->getNameAsString();
            std::string specialization = "template<> void " + templateName + "<int>(int val) {\n";
            specialization += "    static_assert(sizeof(int) > 4, \"Unexpected int size\");\n";
            specialization += "    // specialized logic for int\n";
            specialization += "}\n";
            Rewrite.InsertTextAfterToken(FT->getTemplatedDecl()->getEndLoc(), "\n/*mut617*/" + specialization);
        }
    }
}
  
void MutatorFrontendAction_617::MutatorASTConsumer_617::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}