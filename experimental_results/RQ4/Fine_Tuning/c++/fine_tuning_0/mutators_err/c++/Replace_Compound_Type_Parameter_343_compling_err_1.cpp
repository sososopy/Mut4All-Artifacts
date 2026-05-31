//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Compound_Type_Parameter_343
 */ 
class MutatorFrontendAction_343 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(343)

private:
    class MutatorASTConsumer_343 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_343(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Compound_Type_Parameter_343.h"

// ========================================================================================================
#define MUT343_OUTPUT 1

void MutatorFrontendAction_343::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isImplicit())
        return;
      if (FD->isMain())
        return;
      auto params = FD->parameters();
      if (params.empty())
        return;
      auto param = params[getrandom::getRandomIndex(params.size() - 1)];
      auto paramtype = param->getType();
      auto type = paramtype->getTypePtr();
      if (type->isBuiltinType()) {
        auto newtype = getrandom::getRandomBuiltinType();
        while (newtype == paramtype)
          newtype = getrandom::getRandomBuiltinType();
        llvm::outs() << "/*mut343*/" << newtype.getAsString() << '\n';
        Rewrite.ReplaceText(param->getBeginLoc(),
                            param->getEndLoc().getRawEncoding() -
                                param->getBeginLoc().getRawEncoding() + 1,
                            newtype.getAsString() + " " + param->getNameAsString());
        return;
      }
      if (type->isStructureType() || type->isClassType() ||
          type->isUnionType()) {
        auto all_compound_types = getrandom::getAllCompoundTypes();
        auto newtype = all_compound_types[getrandom::getRandomIndex(
            all_compound_types.size() - 1)];
        while (newtype == paramtype)
          newtype = all_compound_types[getrandom::getRandomIndex(
              all_compound_types.size() - 1)];
        llvm::outs() << "/*mut343*/" << newtype.getAsString() << '\n';
        Rewrite.ReplaceText(param->getBeginLoc(),
                            param->getEndLoc().getRawEncoding() -
                                param->getBeginLoc().getRawEncoding() + 1,
                            newtype.getAsString() + " " + param->getNameAsString());
        return;
      }
    }
}
  
void MutatorFrontendAction_343::MutatorASTConsumer_343::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}