//header file
#pragma once
#include "Mutator_base.h"

/**
 * transparent_union_incomplete_type_34
 */ 
class MutatorFrontendAction_34 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(34)

private:
    class MutatorASTConsumer_34 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_34(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/transparent_union_incomplete_type_34.h"

// ========================================================================================================
#define MUT34_OUTPUT 1

void MutatorFrontendAction_34::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TypedefDecl>("TransparentUnion")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (auto *UT = TD->getUnderlyingType()->getAs<clang::RecordType>()) {
            if (UT->getDecl()->hasAttr<clang::TransparentUnionAttr>()) {
                auto declaration = stringutils::rangetoStr(*(Result.SourceManager), TD->getSourceRange());
                size_t pos = declaration.find("{");
                if (pos != std::string::npos) {
                    std::string mutatedUnion = "\nunion IncompleteType member;";
                    declaration.insert(pos + 1, mutatedUnion);
                    Rewrite.ReplaceText(CharSourceRange::getTokenRange(TD->getSourceRange()), declaration);
                }
            }
        }
    }
}

void MutatorFrontendAction_34::MutatorASTConsumer_34::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = typedefDecl(hasType(recordType(hasAttr(clang::attr::TransparentUnion)))).bind("TransparentUnion");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}