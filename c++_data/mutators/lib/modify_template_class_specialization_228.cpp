//source file
#include "../include/Modify_Template_Class_Specialization_228.h"

// ========================================================================================================
#define MUT228_OUTPUT 1

void MutatorFrontendAction_228::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                      TD->getLocation()))
            return;

        for (auto *Spec : TD->specializations()) {
            if (Spec->isExplicitSpecialization()) {
                std::string specializationText = "template<> class " + TD->getNameAsString() + "<int, void>; /*mut228*/\n";
                SourceLocation insertLoc = Spec->getEndLoc().getLocWithOffset(1);
                Rewrite.InsertText(insertLoc, specializationText, true, true);
                break;
            }
        }
    }
}

void MutatorFrontendAction_228::MutatorASTConsumer_228::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(hasName("basic_ios")).bind("TemplateClass");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}