//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Invalid_Attribute_368
 */ 
class MutatorFrontendAction_368 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(368)

private:
    class MutatorASTConsumer_368 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_368(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Invalid_Attribute_368.h"

// ========================================================================================================
#define MUT368_OUTPUT 1

void MutatorFrontendAction_368::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *D = Result.Nodes.getNodeAs<clang::Decl>("declWithAttr")) {
        if (!D || !Result.Context->getSourceManager().isWrittenInMainFile(D->getLocation()))
            return;

        if (auto *FD = dyn_cast<clang::FunctionDecl>(D)) {
            auto attrs = FD->getAttrs();
            if (attrs.empty()) return;
            int index = getrandom::getRandomIndex(attrs.size());
            const clang::Attr *selectedAttr = attrs[index];
            SourceRange attrRange = selectedAttr->getRange();
            std::string newAttrName = getrandom::getRandomInvalidAttrName();
            std::string newAttrText = "[[" + newAttrName + "]]";
            Rewrite.ReplaceText(attrRange, newAttrText);
        } else if (auto *VD = dyn_cast<clang::VarDecl>(D)) {
            auto attrs = VD->getAttrs();
            if (attrs.empty()) return;
            int index = getrandom::getRandomIndex(attrs.size());
            const clang::Attr *selectedAttr = attrs[index];
            SourceRange attrRange = selectedAttr->getRange();
            std::string newAttrName = getrandom::getRandomInvalidAttrName();
            std::string newAttrText = "[[" + newAttrName + "]]";
            Rewrite.ReplaceText(attrRange, newAttrText);
        } else if (auto *RD = dyn_cast<clang::CXXRecordDecl>(D)) {
            auto attrs = RD->getAttrs();
            if (attrs.empty()) return;
            int index = getrandom::getRandomIndex(attrs.size());
            const clang::Attr *selectedAttr = attrs[index];
            SourceRange attrRange = selectedAttr->getRange();
            std::string newAttrName = getrandom::getRandomInvalidAttrName();
            std::string newAttrText = "[[" + newAttrName + "]]";
            Rewrite.ReplaceText(attrRange, newAttrText);
        }
    }
}
  
void MutatorFrontendAction_368::MutatorASTConsumer_368::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = decl(hasAttr(attr())).bind("declWithAttr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}