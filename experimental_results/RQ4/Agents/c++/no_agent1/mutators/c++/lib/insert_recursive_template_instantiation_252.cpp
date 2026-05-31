//source file
#include "../include/insert_recursive_template_instantiation_252.h"

// ========================================================================================================
#define MUT252_OUTPUT 1

void MutatorFrontendAction_252::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;

        templates.push_back(CTD);

        if (templates.size() >= 2) {
            const auto *template1 = templates[0];
            const auto *template2 = templates[1];

            std::string template1Name = template1->getNameAsString();
            std::string template2Name = template2->getNameAsString();

            std::string instantiation = "template<> struct " + template1Name + "<" + template2Name + "<" + template1Name + "<int>>> {};\n";
            instantiation = "/*mut252*/" + instantiation;

            SourceLocation insertLoc = template2->getEndLoc();
            Rewrite.InsertTextAfterToken(insertLoc, instantiation);
        }
    }
}

void MutatorFrontendAction_252::MutatorASTConsumer_252::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("Templates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}