//source file
#include "../include/insert_recursive_template_instantiation_166.h"

// ========================================================================================================
#define MUT166_OUTPUT 1

void MutatorFrontendAction_166::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CTD->getLocation()))
            return;
        
        if (processedTemplates.find(CTD) != processedTemplates.end())
            return;

        processedTemplates.insert(CTD);

        auto templateName = CTD->getNameAsString();
        std::string recursiveInstantiation = "template<> class " + templateName + "<0> { public: static const int value = " + templateName + "<1>::value; };";
        std::string instantiation = "\n/*mut166*/" + recursiveInstantiation + "\n";

        SourceLocation insertLoc = CTD->getEndLoc();
        Rewrite.InsertTextAfterToken(insertLoc, instantiation);
    }
}

void MutatorFrontendAction_166::MutatorASTConsumer_166::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}