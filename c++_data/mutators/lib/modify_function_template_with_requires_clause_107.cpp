//source file
#include "../include/modify_function_template_with_requires_clause_107.h"

// ========================================================================================================
#define MUT107_OUTPUT 1

void MutatorFrontendAction_107::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getTemplatedDecl()->getBody()) {
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            size_t requiresPos = declaration.find("requires");
            if (requiresPos != std::string::npos) {
                size_t paramPos = declaration.find(")", requiresPos);
                if (paramPos != std::string::npos) {
                    declaration.insert(paramPos, ", auto extraParam");
                    size_t clausePos = declaration.find(")", paramPos);
                    if (clausePos != std::string::npos) {
                        declaration.insert(clausePos, " && extraParam()");
                    }
                }
            }
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
    }
}
  
void MutatorFrontendAction_107::MutatorASTConsumer_107::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}