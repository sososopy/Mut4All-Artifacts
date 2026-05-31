//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Forward_Iterator_With_Reverse_Iterator_In_Generic_Lambda_Iteration_138
 */ 
class MutatorFrontendAction_138 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(138)

private:
    class MutatorASTConsumer_138 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_138(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl *> containerDecls;
    };
};

//source file
#include "../include/Mutator_Replace_Forward_Iterator_With_Reverse_Iterator_In_Generic_Lambda_Iteration_138.h"

// ========================================================================================================
#define MUT138_OUTPUT 1

void MutatorFrontendAction_138::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("ContainerVar")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if the variable is a container that supports reverse iterators
      const clang::Type *type = VD->getType().getTypePtrOrNull();
      if (!type) return;
      if (type->isPointerType() || type->isArrayType()) return;
      //Record container variable declarations for later use
      containerDecls.push_back(VD);
    }
    else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("LambdaCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getLocation()))
        return;
      //Get the source code text of target node
      auto callRange = CharSourceRange::getTokenRange(CE->getSourceRange());
      std::string callText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());
      //Check if this is a call to a lambda with two arguments
      if (CE->getNumArgs() != 2) return;
      clang::Expr *arg1 = CE->getArg(0);
      clang::Expr *arg2 = CE->getArg(1);
      if (!arg1 || !arg2) return;
      //Check if arguments are iterator expressions (begin/end)
      std::string arg1Text = stringutils::rangetoStr(*(Result.SourceManager), arg1->getSourceRange());
      std::string arg2Text = stringutils::rangetoStr(*(Result.SourceManager), arg2->getSourceRange());
      //Look for .begin() and .end() patterns
      size_t beginPos1 = arg1Text.find(".begin()");
      size_t endPos1 = arg1Text.find(".end()");
      size_t beginPos2 = arg2Text.find(".begin()");
      size_t endPos2 = arg2Text.find(".end()");
      //We need one argument to be .begin() and the other to be .end()
      if (!((beginPos1 != std::string::npos && endPos2 != std::string::npos) ||
            (beginPos2 != std::string::npos && endPos1 != std::string::npos))) return;
      //Determine which argument is begin and which is end
      bool firstIsBegin = (beginPos1 != std::string::npos);
      std::string containerName;
      if (firstIsBegin) {
        containerName = arg1Text.substr(0, beginPos1);
      } else {
        containerName = arg2Text.substr(0, beginPos2);
      }
      //Trim whitespace
      containerName.erase(0, containerName.find_first_not_of(" \t\n\r"));
      containerName.erase(containerName.find_last_not_of(" \t\n\r") + 1);
      //Check if we have a recorded container declaration matching this name
      const clang::VarDecl *matchingContainer = nullptr;
      for (const auto *container : containerDecls) {
        if (container->getNameAsString() == containerName) {
          matchingContainer = container;
          break;
        }
      }
      if (!matchingContainer) return;
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCall = callText;
      if (firstIsBegin) {
        size_t pos = mutatedCall.find(arg1Text);
        if (pos != std::string::npos) {
          mutatedCall.replace(pos, arg1Text.length(), containerName + ".rbegin()");
        }
        pos = mutatedCall.find(arg2Text);
        if (pos != std::string::npos) {
          mutatedCall.replace(pos, arg2Text.length(), containerName + ".rend()");
        }
      } else {
        size_t pos = mutatedCall.find(arg2Text);
        if (pos != std::string::npos) {
          mutatedCall.replace(pos, arg2Text.length(), containerName + ".rbegin()");
        }
        pos = mutatedCall.find(arg1Text);
        if (pos != std::string::npos) {
          mutatedCall.replace(pos, arg1Text.length(), containerName + ".rend()");
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(callRange, mutatedCall);
    }
    else if (auto *L = Result.Nodes.getNodeAs<clang::LambdaExpr>("LambdaBody")) {
      //Filter nodes in header files
      if (!L || !Result.Context->getSourceManager().isWrittenInMainFile(
                     L->getLocation()))
        return;
      //Check if lambda has a body
      clang::Stmt *body = L->getBody();
      if (!body) return;
      //Get the source code text of target node
      std::string bodyText = stringutils::rangetoStr(*(Result.SourceManager), body->getSourceRange());
      //Look for iterator-based loops in the lambda body
      //Search for patterns like "begin()" and "end()"
      size_t beginPos = bodyText.find(".begin()");
      size_t endPos = bodyText.find(".end()");
      if (beginPos == std::string::npos || endPos == std::string::npos) return;
      //Find the container name before .begin()
      size_t containerStart = bodyText.rfind(' ', beginPos);
      if (containerStart == std::string::npos) containerStart = 0;
      else containerStart++; // Skip the space
      std::string containerName = bodyText.substr(containerStart, beginPos - containerStart);
      //Trim whitespace
      containerName.erase(0, containerName.find_first_not_of(" \t\n\r"));
      containerName.erase(containerName.find_last_not_of(" \t\n\r") + 1);
      //Check if we have a recorded container declaration matching this name
      const clang::VarDecl *matchingContainer = nullptr;
      for (const auto *container : containerDecls) {
        if (container->getNameAsString() == containerName) {
          matchingContainer = container;
          break;
        }
      }
      if (!matchingContainer) return;
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedBody = bodyText;
      //Replace .begin() with .rbegin()
      size_t pos = mutatedBody.find(".begin()");
      while (pos != std::string::npos) {
        //Check if this is our container's begin()
        size_t checkStart = pos;
        while (checkStart > 0 && (isalnum(mutatedBody[checkStart-1]) || mutatedBody[checkStart-1] == '_' || mutatedBody[checkStart-1] == '.')) {
          checkStart--;
        }
        std::string prefix = mutatedBody.substr(checkStart, pos - checkStart);
        if (prefix.find(containerName) != std::string::npos) {
          mutatedBody.replace(pos, 8, ".rbegin()");
        }
        pos = mutatedBody.find(".begin()", pos + 9);
      }
      //Replace .end() with .rend()
      pos = mutatedBody.find(".end()");
      while (pos != std::string::npos) {
        //Check if this is our container's end()
        size_t checkStart = pos;
        while (checkStart > 0 && (isalnum(mutatedBody[checkStart-1]) || mutatedBody[checkStart-1] == '_' || mutatedBody[checkStart-1] == '.')) {
          checkStart--;
        }
        std::string prefix = mutatedBody.substr(checkStart, pos - checkStart);
        if (prefix.find(containerName) != std::string::npos) {
          mutatedBody.replace(pos, 6, ".rend()");
        }
        pos = mutatedBody.find(".end()", pos + 7);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), mutatedBody);
    }
}
  
void MutatorFrontendAction_138::MutatorASTConsumer_138::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //Match container variable declarations (vectors, lists, etc.)
    DeclarationMatcher containerMatcher = varDecl(hasType(recordDecl(hasAnyName("std::vector", "std::list", "std::deque", "std::array", "std::set", "std::map")))).bind("ContainerVar");
    //Match lambda calls with two arguments
    StatementMatcher lambdaCallMatcher = callExpr(hasArgument(0, expr()), hasArgument(1, expr()), callee(expr())).bind("LambdaCall");
    //Match lambda expressions themselves
    StatementMatcher lambdaMatcher = lambdaExpr().bind("LambdaBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(containerMatcher, &callback);
    matchFinder.addMatcher(lambdaCallMatcher, &callback);
    matchFinder.addMatcher(lambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}