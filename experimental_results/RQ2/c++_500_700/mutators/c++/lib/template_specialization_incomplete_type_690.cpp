//source file
#include "../include/template_specialization_incomplete_type_690.h"

// ========================================================================================================
#define MUT690_OUTPUT 1

void MutatorFrontendAction_690::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        std::string templateName = TD->getNameAsString();
        std::string specialization = "class UndefinedType; // Forward declaration only\n";
        specialization += "template<> class " + templateName + "<UndefinedType>;\n";
        
        SourceLocation insertLoc = TD->getEndLoc().getLocWithOffset(1);
        Rewrite.InsertText(insertLoc, "\n/*mut690*/" + specialization);
    }
}

void MutatorFrontendAction_690::MutatorASTConsumer_690::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}