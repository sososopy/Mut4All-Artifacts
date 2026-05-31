//header file
#pragma once
#include "Mutator_base.h"

/**
 * Namespace_Nesting_Anomalies_223
 */ 
class MutatorFrontendAction_223 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(223)

private:
    class MutatorASTConsumer_223 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_223(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Namespace_Nesting_Anomalies_223.h"

// ========================================================================================================
#define MUT223_OUTPUT 1

void MutatorFrontendAction_223::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NSD = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        if (!NSD || !Result.Context->getSourceManager().isWrittenInMainFile(
                        NSD->getLocation()))
            return;

        auto nestedNamespace = NSD->getDeclContext();
        if (nestedNamespace && isa<NamespaceDecl>(nestedNamespace)) {
            auto sourceRange = NSD->getSourceRange();
            auto text = stringutils::rangetoStr(*(Result.SourceManager), sourceRange);

            if (getrandom::getRandomIndex(2) == 0) {
                text.insert(text.find("{") + 1, ";");
            } else {
                auto pos = text.rfind("}");
                if (pos != std::string::npos) {
                    text.erase(pos, 1);
                }
            }

            Rewrite.ReplaceText(CharSourceRange::getTokenRange(sourceRange), text);
        }
    }
}
  
void MutatorFrontendAction_223::MutatorASTConsumer_223::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = namespaceDecl(hasAncestor(namespaceDecl())).bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}