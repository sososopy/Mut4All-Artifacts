//source file
#include "../include/Mutator_214.h"

// ========================================================================================================
#define MUT214_OUTPUT 1

void MutatorFrontendAction_214::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;

        std::string className = CTD->getNameAsString();
        if (specializedClasses.find(className) != specializedClasses.end())
            return;

        specializedClasses.insert(className);

        std::string specialization1 = "template<typename T>\nclass " + className + "<T*> {\n/* Specialized class definition */\n};\n";
        std::string specialization2 = "template<typename T>\nclass " + className + "<T**> {\n/* Conflicting specialization */\n};\n";

        SourceLocation insertLoc = CTD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertTextAfter(insertLoc, "\n/*mut214*/" + specialization1 + specialization2);
    }
}
  
void MutatorFrontendAction_214::MutatorASTConsumer_214::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}