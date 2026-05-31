//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Default_Argument_Invalid_Forward_Reference_161
 */ 
class MutatorFrontendAction_161 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(161)

private:
    class MutatorASTConsumer_161 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_161(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
        std::vector<const clang::ParmVarDecl*> parameters;
    };
};

//source file
#include "../include/Mutator_Default_Argument_Invalid_Forward_Reference_161.h"

// ========================================================================================================
#define MUT161_OUTPUT 1

void MutatorFrontendAction_161::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (!FD->hasBody() || FD->getNumParams() == 0)
        return;
      parameters.clear();
      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
        parameters.push_back(FD->getParamDecl(i));
      }
      for (unsigned i = 0; i < FD->getNumParams(); ++i) {
        const clang::ParmVarDecl* param = FD->getParamDecl(i);
        if (param->hasDefaultArg()) {
          clang::Expr* defaultArg = param->getDefaultArg();
          if (!defaultArg)
            continue;
          std::string defaultArgText = stringutils::rangetoStr(*(Result.SourceManager), defaultArg->getSourceRange());
          //(optional)Record the node information to be used in the mutation process
          std::string mutatedArg = defaultArgText;
          bool mutated = false;
          if (clang::CallExpr* callExpr = llvm::dyn_cast<clang::CallExpr>(defaultArg)) {
            clang::Expr* callee = callExpr->getCallee()->IgnoreImpCasts();
            if (clang::DeclRefExpr* declRef = llvm::dyn_cast<clang::DeclRefExpr>(callee)) {
              std::string calleeName = declRef->getNameInfo().getAsString();
              for (const clang::ParmVarDecl* p : parameters) {
                if (p->getNameAsString() == calleeName) {
                  mutatedArg = parameters[0]->getNameAsString() + "()";
                  mutated = true;
                  break;
                }
              }
            }
          } else if (clang::DeclRefExpr* declRef = llvm::dyn_cast<clang::DeclRefExpr>(defaultArg)) {
            std::string refName = declRef->getNameInfo().getAsString();
            for (const clang::ParmVarDecl* p : parameters) {
              if (p->getNameAsString() == refName) {
                mutatedArg = parameters[0]->getNameAsString();
                mutated = true;
                break;
              }
            }
          } else if (clang::BinaryOperator* binOp = llvm::dyn_cast<clang::BinaryOperator>(defaultArg)) {
            std::string lhsText = stringutils::rangetoStr(*(Result.SourceManager), binOp->getLHS()->getSourceRange());
            std::string rhsText = stringutils::rangetoStr(*(Result.SourceManager), binOp->getRHS()->getSourceRange());
            for (const clang::ParmVarDecl* p : parameters) {
              std::string pName = p->getNameAsString();
              if (lhsText.find(pName) != std::string::npos || rhsText.find(pName) != std::string::npos) {
                mutatedArg = parameters[0]->getNameAsString() + " " + binOp->getOpcodeStr().str() + " " + parameters[0]->getNameAsString();
                mutated = true;
                break;
              }
            }
          }
          if (!mutated) {
            if (clang::MemberExpr* memberExpr = llvm::dyn_cast<clang::MemberExpr>(defaultArg)) {
              mutatedArg = parameters[0]->getNameAsString() + "." + memberExpr->getMemberNameInfo().getAsString() + "()";
              mutated = true;
            } else {
              mutatedArg = parameters[0]->getNameAsString();
              mutated = true;
            }
          }
          //Perform mutation on the source code text by applying string replacement
          if (mutated) {
            std::string fullDecl = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
            size_t paramPos = fullDecl.find(param->getNameAsString());
            if (paramPos != std::string::npos) {
              size_t defaultStart = fullDecl.find('=', paramPos);
              if (defaultStart != std::string::npos) {
                size_t defaultEnd = fullDecl.find_first_of(",)", defaultStart);
                if (defaultEnd != std::string::npos) {
                  std::string newDecl = fullDecl.substr(0, defaultStart + 1) + " " + mutatedArg + fullDecl.substr(defaultEnd);
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), "/*mut161*/" + newDecl);
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_161::MutatorASTConsumer_161::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasDefaultArg())).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}