//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_union_initializer_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FieldDecl *firstField = nullptr;
    };
};

//source file
#include "../include/modify_union_initializer_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *InitExpr = Result.Nodes.getNodeAs<clang::InitListExpr>("initList")) {
        //Filter nodes in header files
        if (!InitExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                InitExpr->getBeginLoc()))
            return;

        if (auto *UnionDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("unionDecl")) {
            if (!UnionDecl->isUnion())
                return;

            if (firstField == nullptr) {
                for (const auto *Field : UnionDecl->fields()) {
                    if (Field->getType()->isBuiltinType()) {
                        firstField = Field;
                        break;
                    }
                }
            }

            if (firstField) {
                //Get the source code text of target node
                std::string initText = "{ .";
                initText += firstField->getNameAsString();
                initText += " = 0, ";

                for (const auto *Field : UnionDecl->fields()) {
                    if (Field != firstField) {
                        initText += ".";
                        initText += Field->getNameAsString();
                        initText += " = 0, ";
                        break;
                    }
                }

                initText.pop_back(); // remove last comma
                initText.pop_back();
                initText += " }";

                //Perform mutation on the source code text by applying string replacement
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitExpr->getSourceRange()), initText);
            }
        }
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto unionMatcher = cxxRecordDecl(isUnion()).bind("unionDecl");
    auto initListMatcher = initListExpr(hasType(recordDecl(isUnion()))).bind("initList");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(unionMatcher, &callback);
    matchFinder.addMatcher(initListMatcher, &callback);
    matchFinder.matchAST(Context);
}