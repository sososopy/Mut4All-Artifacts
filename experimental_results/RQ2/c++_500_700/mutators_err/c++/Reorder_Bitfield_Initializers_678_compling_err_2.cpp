//header file
#pragma once
#include "Mutator_base.h"

/**
 * reorder_bitfield_initializers_678
 */ 
class MutatorFrontendAction_678 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(678)

private:
    class MutatorASTConsumer_678 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_678(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/reorder_bitfield_initializers_678.h"

// ========================================================================================================
#define MUT678_OUTPUT 1

void MutatorFrontendAction_678::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *InitList = Result.Nodes.getNodeAs<clang::InitListExpr>("InitList")) {
        if (!InitList || !Result.Context->getSourceManager().isWrittenInMainFile(
                InitList->getBeginLoc()))
            return;

        std::vector<std::pair<std::string, std::string>> initializers;
        for (unsigned i = 0; i < InitList->getNumInits(); ++i) {
            if (auto *DesigInit = dyn_cast<DesignatedInitExpr>(InitList->getInit(i))) {
                std::string fieldName = DesigInit->getDesignator(0)->getFieldName()->getName().str();
                std::string initValue = stringutils::rangetoStr(*(Result.SourceManager), DesigInit->getInit()->getSourceRange());
                initializers.emplace_back(fieldName, initValue);
            }
        }

        std::random_shuffle(initializers.begin(), initializers.end());

        std::string reorderedInitList = "{\n";
        for (const auto &init : initializers) {
            reorderedInitList += "." + init.first + " = " + init.second + ",\n";
        }
        reorderedInitList += "}";

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(InitList->getSourceRange()), reorderedInitList);
    }
}

void MutatorFrontendAction_678::MutatorASTConsumer_678::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = initListExpr(hasParent(varDecl(hasType(recordType(hasDeclaration(cxxRecordDecl(isStruct())))))).bind("InitList"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}