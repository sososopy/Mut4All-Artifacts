//header file
#pragma once
#include "Mutator_base.h"

/**
 * Union_Initializer_Mutation_221
 */ 
class MutatorFrontendAction_221 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(221)

private:
    class MutatorASTConsumer_221 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_221(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Union_Initializer_Mutation_221.h"

// ========================================================================================================
#define MUT221_OUTPUT 1

void MutatorFrontendAction_221::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *InitExpr = Result.Nodes.getNodeAs<clang::InitListExpr>("unionInit")) {
        if (!InitExpr || !Result.Context->getSourceManager().isWrittenInMainFile(InitExpr->getBeginLoc()))
            return;

        if (InitExpr->getNumInits() == 1) {
            if (auto *UnionDecl = Result.Nodes.getNodeAs<clang::RecordDecl>("unionDecl")) {
                if (UnionDecl->isUnion() && UnionDecl->field_begin() != UnionDecl->field_end()) {
                    auto InitText = stringutils::rangetoStr(*Result.SourceManager, InitExpr->getSourceRange());
                    auto FieldIter = UnionDecl->field_begin();
                    std::advance(FieldIter, InitExpr->getNumInits());
                    if (FieldIter != UnionDecl->field_end()) {
                        std::string NewInit = InitText;
                        NewInit.insert(NewInit.size() - 1, ", ." + FieldIter->getNameAsString() + " = 0");
                        Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitExpr->getSourceRange()), NewInit);
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_221::MutatorASTConsumer_221::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto unionMatcher = recordDecl(isUnion()).bind("unionDecl");
    auto initListMatcher = initListExpr(hasParent(varDecl(hasType(recordDecl(isUnion()))).bind("varDecl"))).bind("unionInit");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(unionMatcher, &callback);
    matchFinder.addMatcher(initListMatcher, &callback);
    matchFinder.matchAST(Context);
}