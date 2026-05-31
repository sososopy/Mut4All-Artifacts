//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Statement_With_Type_Reference_43
 */ 
class MutatorFrontendAction_43 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(43)

private:
    class MutatorASTConsumer_43 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_43(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/replace_statement_with_type_reference_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                    DL->getLocation()))
        return;
        if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
        return;
        cur_classes.push_back(DL);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Funcs")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                    FD->getLocation()))
        return;
        if (FD->isDefaulted())
        return;
        if (!FD->hasBody())
        return;
        if (cur_classes.empty())
        return;
        auto body = FD->getBody();
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                            body->getSourceRange());
        auto index = getrandom::getRandomIndex(cur_classes.size() - 1);
        auto target = cur_classes[index];
        llvm::outs() << target->getNameAsString() << '\n';
        auto pos = getrandom::getRandomIndex(content.size() - 1);
        auto ins = "/*mut43*/" + target->getNameAsString();
        content.insert(pos, ins);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto func_matcher = functionDecl().bind("Funcs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}