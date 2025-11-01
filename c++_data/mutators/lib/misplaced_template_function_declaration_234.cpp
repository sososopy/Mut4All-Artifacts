//source file
#include "../include/Misplaced_Template_Function_Declaration_234.h"

// ========================================================================================================
#define MUT234_OUTPUT 1

void MutatorFrontendAction_234::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunction")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (auto *RD = dyn_cast<CXXRecordDecl>(TD->getDeclContext())) {
            if (RD->isCompleteDefinition()) {
                currentClass = RD;
                std::string templateText = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
                std::string misplacedText = "\n/*mut234*/" + currentClass->getNameAsString() + "::template " + templateText + ";";
                Rewrite.InsertTextAfterToken(currentClass->getEndLoc(), misplacedText);
                Rewrite.RemoveText(TD->getSourceRange());
            }
        }
    }
}

void MutatorFrontendAction_234::MutatorASTConsumer_234::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(hasParent(cxxRecordDecl())).bind("TemplateFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}