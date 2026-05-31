//source file
#include "../include/Add_Non_Type_Template_Parameter_In_Constructor_428.h"

// ========================================================================================================
#define MUT428_OUTPUT 1

void MutatorFrontendAction_428::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;

        if (!CT->isCompleteDefinition() || CT->isLambda())
            return;

        classTemplates.push_back(CT);
    } else if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
        if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(Ctor->getLocation()))
            return;

        if (Ctor->isImplicit() || !Ctor->isTemplateInstantiation())
            return;

        auto classTemplate = Ctor->getParent();
        if (std::find(classTemplates.begin(), classTemplates.end(), classTemplate) == classTemplates.end())
            return;

        auto sourceRange = Ctor->getSourceRange();
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        std::string mutation = "template<typename T, int N = 0> ";
        mutation += sourceText;
        mutation.insert(mutation.find("{") + 1, "\nstatic_assert(N >= 0, \"N must be non-negative\");\n");

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), mutation);
    }
}

void MutatorFrontendAction_428::MutatorASTConsumer_428::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher classTemplateMatcher = cxxRecordDecl(isTemplateInstantiation()).bind("ClassTemplate");
    DeclarationMatcher constructorMatcher = cxxConstructorDecl(isTemplateInstantiation()).bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classTemplateMatcher, &callback);
    matchFinder.addMatcher(constructorMatcher, &callback);
    matchFinder.matchAST(Context);
}