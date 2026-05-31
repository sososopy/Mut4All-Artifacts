//source file
#include "../include/introduce_openmp_in_structured_binding_394.h"

// ========================================================================================================
#define MUT394_OUTPUT 1

void MutatorFrontendAction_394::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *BD = Result.Nodes.getNodeAs<clang::DecompositionDecl>("bindingDecl")) {
        if (!BD || !Result.Context->getSourceManager().isWrittenInMainFile(BD->getLocation()))
            return;

        structuredBindingDecl = BD;
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (structuredBindingDecl) {
            auto body = FD->getBody();
            if (!body)
                return;

            auto funcText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
            std::string pragmaText = "\n#pragma omp parallel\n{\n";
            std::string closingBrace = "\n}\n";

            size_t insertPos = funcText.find('{') + 1;
            funcText.insert(insertPos, pragmaText);
            funcText.append(closingBrace);

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), funcText);
        }
    }
}

void MutatorFrontendAction_394::MutatorASTConsumer_394::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto bindingMatcher = decompositionDecl(hasType(recordDecl(hasName("CustomStruct")))).bind("bindingDecl");
    auto functionMatcher = functionDecl(hasDescendant(decompositionDecl(hasType(recordDecl(hasName("CustomStruct")))))).bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(bindingMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}