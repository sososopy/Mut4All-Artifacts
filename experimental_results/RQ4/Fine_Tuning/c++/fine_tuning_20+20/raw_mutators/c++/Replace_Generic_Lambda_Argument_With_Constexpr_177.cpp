//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Generic_Lambda_Argument_With_Constexpr_177
 */ 
class MutatorFrontendAction_177 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(177)

private:
    class MutatorASTConsumer_177 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_177(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> constexpr_int_vars;
    };
};

//source file
#include "../include/replace_generic_lambda_argument_with_constexpr_177.h"

// ========================================================================================================
#define MUT177_OUTPUT 1

void MutatorFrontendAction_177::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ConstexprInt")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->isConstexpr())
        return;
      if (!VD->getType()->isIntegerType())
        return;
      constexpr_int_vars.push_back(VD);
    } else if (auto *LD =
                   Result.Nodes.getNodeAs<clang::LambdaExpr>("GenericLambda")) {
      if (!LD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LD->getBeginLoc()))
        return;
      if (constexpr_int_vars.empty())
        return;
      auto lambda_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    LD->getSourceRange());
      llvm::outs() << lambda_content << '\n';
      auto param_list = LD->getLambdaClass()->getLambdaTypeInfo()->getParams();
      if (param_list.size() != 1)
        return;
      auto param = param_list[0];
      if (!param->hasTypeConstraint())
        return;
      if (param->getTypeConstraint()->getName() != "auto")
        return;
      size_t index = getrandom::getRandomIndex(constexpr_int_vars.size() - 1);
      auto target = constexpr_int_vars[index];
      auto target_name = target->getNameAsString();
      auto target_type = target->getType().getAsString();
      llvm::outs() << target_name << ' ' << target_type << '\n';
      string replacement = "[&]() -> bool { return " + target_name + "==0; }";
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LD->getSourceRange()),
                          replacement);
    }
}
  
void MutatorFrontendAction_177::MutatorASTConsumer_177::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto constexpr_int_matcher =
        varDecl(hasType(isInteger()), isConstexpr()).bind("ConstexprInt");
    auto generic_lambda_matcher = lambdaExpr(hasAnyTemplateArgument(refersToType(asString("auto")))).bind("GenericLambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(constexpr_int_matcher, &callback);
    matchFinder.addMatcher(generic_lambda_matcher, &callback);
    matchFinder.matchAST(Context);
}