//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Identifier_Character_With_Universal_Escape_457
 */ 
class MutatorFrontendAction_457 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(457)

private:
    class MutatorASTConsumer_457 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_457(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Identifier_Character_With_Universal_Escape_457.h"

// ========================================================================================================
#define MUT457_OUTPUT 1

void MutatorFrontendAction_457::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *D = Result.Nodes.getNodeAs<clang::DeclaratorDecl>("var")) {
        if (!D || !Result.Context->getSourceManager().isWrittenInMainFile(D->getLocation()))
            return;
        std::string originalName = D->getNameAsString();
        if (originalName.empty())
            return;
        size_t length = originalName.length();
        if (length == 0)
            return;
        int index = getrandom::getRandomIndex(length);
        if (index < 0 || index >= static_cast<int>(length))
            return;
        char c = originalName[index];
        uint32_t codePoint = static_cast<uint32_t>(c);
        char buffer[5];
        snprintf(buffer, sizeof(buffer), "%04x", codePoint);
        std::string hex(buffer);
        std::string escape = "\\u" + hex;
        std::string modifiedName = originalName;
        modifiedName.replace(index, 1, escape);
        Rewrite.ReplaceText(D->getLocation(), originalName.length(), modifiedName);
    }
}
  
void MutatorFrontendAction_457::MutatorASTConsumer_457::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = (varDecl().bind("var") || parmVarDecl().bind("var"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}