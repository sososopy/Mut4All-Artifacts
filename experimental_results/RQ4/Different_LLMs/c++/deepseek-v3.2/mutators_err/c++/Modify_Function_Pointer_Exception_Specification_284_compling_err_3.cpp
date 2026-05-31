//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Pointer_Exception_Specification_284
 */ 
class MutatorFrontendAction_284 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(284)

private:
    class MutatorASTConsumer_284 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_284(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Mutator_Modify_Function_Pointer_Exception_Specification_284.h"

// ========================================================================================================
#define MUT284_OUTPUT 1

void MutatorFrontendAction_284::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      auto *Type = FD->getType().getTypePtr();
      if (!Type->isFunctionPointerType() && !Type->isFunctionType())
        return;
      auto SourceRange = FD->getSourceRange();
      auto OriginalText = stringutils::rangetoStr(*(Result.SourceManager), SourceRange);
      //(optional)Record the node information to be used in the mutation process
      auto *FunctionType = Type->getAs<clang::FunctionType>();
      auto ExceptionSpec = FunctionType->getExceptionSpecType();
      std::string NewExceptionSpec;
      int Choice = getrandom::getRandomIndex(3);
      if (ExceptionSpec == clang::ExceptionSpecificationType::EST_None) {
        if (Choice == 0) NewExceptionSpec = "noexcept";
        else if (Choice == 1) NewExceptionSpec = "noexcept(false)";
        else NewExceptionSpec = "throw()";
      } else if (ExceptionSpec == clang::ExceptionSpecificationType::EST_Noexcept) {
        if (Choice == 0) NewExceptionSpec = "";
        else if (Choice == 1) NewExceptionSpec = "noexcept(false)";
        else NewExceptionSpec = "throw()";
      } else if (ExceptionSpec == clang::ExceptionSpecificationType::EST_Dynamic) {
        if (Choice == 0) NewExceptionSpec = "";
        else if (Choice == 1) NewExceptionSpec = "noexcept";
        else NewExceptionSpec = "noexcept(false)";
      } else if (ExceptionSpec == clang::ExceptionSpecificationType::EST_NoexceptFalse) {
        if (Choice == 0) NewExceptionSpec = "";
        else if (Choice == 1) NewExceptionSpec = "noexcept";
        else NewExceptionSpec = "throw()";
      } else {
        return;
      }
      //Perform mutation on the source code text by applying string replacement
      std::string MutatedText;
      if (NewExceptionSpec.empty()) {
        size_t Pos = OriginalText.find("noexcept");
        if (Pos != std::string::npos) {
          MutatedText = OriginalText.substr(0, Pos);
          if (OriginalText.find("noexcept(false)", Pos) != std::string::npos) {
            MutatedText += OriginalText.substr(Pos + 15);
          } else {
            MutatedText += OriginalText.substr(Pos + 8);
          }
        } else {
          Pos = OriginalText.find("throw()");
          if (Pos != std::string::npos) {
            MutatedText = OriginalText.substr(0, Pos);
            MutatedText += OriginalText.substr(Pos + 7);
          } else {
            return;
          }
        }
      } else {
        size_t Pos = OriginalText.find("noexcept");
        if (Pos != std::string::npos) {
          if (OriginalText.find("noexcept(false)", Pos) != std::string::npos) {
            MutatedText = OriginalText.substr(0, Pos);
            MutatedText += NewExceptionSpec;
            MutatedText += OriginalText.substr(Pos + 15);
          } else {
            MutatedText = OriginalText.substr(0, Pos);
            MutatedText += NewExceptionSpec;
            MutatedText += OriginalText.substr(Pos + 8);
          }
        } else {
          Pos = OriginalText.find("throw()");
          if (Pos != std::string::npos) {
            MutatedText = OriginalText.substr(0, Pos);
            MutatedText += NewExceptionSpec;
            MutatedText += OriginalText.substr(Pos + 7);
          } else {
            size_t ParenPos = OriginalText.rfind(')');
            if (ParenPos != std::string::npos) {
              MutatedText = OriginalText.substr(0, ParenPos + 1);
              MutatedText += " " + NewExceptionSpec;
              MutatedText += OriginalText.substr(ParenPos + 1);
            } else {
              return;
            }
          }
        }
      }
      MutatedText = "/*mut284*/" + MutatedText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SourceRange), MutatedText);
    }
}
  
void MutatorFrontendAction_284::MutatorASTConsumer_284::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = fieldDecl(hasParent(recordDecl(isStruct(), isClass(), isUnion())), hasType(functionType())).bind("FieldDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}