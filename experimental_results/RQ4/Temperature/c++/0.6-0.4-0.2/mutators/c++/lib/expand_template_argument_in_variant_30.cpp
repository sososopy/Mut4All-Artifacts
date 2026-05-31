//source file
#include "../include/expand_template_argument_in_variant_30.h"

// ========================================================================================================
#define MUT30_OUTPUT 1

void MutatorFrontendAction_30::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TU = Result.Nodes.getNodeAs<clang::TypedefDecl>("TemplateUsing")) {
        if (!TU || !Result.Context->getSourceManager().isWrittenInMainFile(TU->getLocation()))
            return;

        auto templateDecl = dyn_cast<ClassTemplateSpecializationDecl>(TU->getUnderlyingType()->getAsCXXRecordDecl());
        if (!templateDecl)
            return;

        // Get the source code text of target node
        auto sourceRange = TU->getSourceRange();
        auto sourceText = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

        // Perform mutation on the source code text by applying string replacement
        std::string newText = "template <typename... Ts>\nusing " + TU->getNameAsString() + " = std::variant<Ts...>;\n";

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), newText);
    }
}

void MutatorFrontendAction_30::MutatorASTConsumer_30::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typedefDecl(hasType(asString("std::variant<int, double, char>"))).bind("TemplateUsing");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}