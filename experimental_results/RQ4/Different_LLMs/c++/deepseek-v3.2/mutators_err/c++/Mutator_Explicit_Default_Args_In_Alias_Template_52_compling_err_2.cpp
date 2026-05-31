//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_Explicit_Default_Args_In_Alias_Template_52
 */ 
class MutatorFrontendAction_52 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(52)

private:
    class MutatorASTConsumer_52 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_52(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Explicit_Default_Args_In_Alias_Template_52.h"

// ========================================================================================================
#define MUT52_OUTPUT 1

void MutatorFrontendAction_52::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TA = Result.Nodes.getNodeAs<clang::TypeAliasTemplateDecl>("TypeAliasTemplateDecl")) {
      //Filter nodes in header files
      if (!TA || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TA->getLocation()))
        return;
      //Get the underlying type alias declaration
      auto *TAD = TA->getTemplatedDecl();
      if (!TAD) return;
      //Get the aliased type
      auto *UnderlyingType = TAD->getUnderlyingType().getTypePtr();
      if (!UnderlyingType) return;
      //Check if it's a template specialization
      auto *TST = dyn_cast<TemplateSpecializationType>(UnderlyingType);
      if (!TST) return;
      //Get the template declaration
      auto *TemplateDecl = TST->getTemplateName().getAsTemplateDecl();
      if (!TemplateDecl) return;
      auto *CTD = dyn_cast<ClassTemplateDecl>(TemplateDecl);
      if (!CTD) return;
      //Check if the class template has any templated constructor
      bool HasTemplatedConstructor = false;
      for (auto *Ctor : CTD->getTemplatedDecl()->ctors()) {
        if (Ctor->getNumTemplateParameterLists() > 0) {
          HasTemplatedConstructor = true;
          break;
        }
      }
      if (!HasTemplatedConstructor) return;
      //Get the template arguments from the alias
      auto TemplateArgs = TST->template_arguments();
      //Get the template parameters of the alias template
      auto *TP = TA->getTemplateParameters();
      if (!TP) return;
      //Get the template parameters of the class template
      auto *ClassTP = CTD->getTemplateParameters();
      if (!ClassTP) return;
      //Check if any class template parameter has a default argument
      bool HasDefaultArg = false;
      for (unsigned i = 0; i < ClassTP->size(); ++i) {
        auto *P = ClassTP->getParam(i);
        if (P->hasDefaultArg()) {
          HasDefaultArg = true;
          break;
        }
      }
      if (!HasDefaultArg)  return;
      //Now we need to mutate: for each template argument in the alias that uses default,
      //replace with explicit default value
      //First get the source range of the aliased type (the part after '=')
      auto SourceRange = TAD->getSourceRange();
      if (SourceRange.isInvalid()) return;
      //Get the location of '='
      auto EqualLoc = TAD->getLocation();
      //Find the start of the aliased type (after '=')
      SourceManager &SM = Rewrite.getSourceMgr();
      const char *SourceStart = SM.getCharacterData(EqualLoc);
      //Skip '=' and whitespace
      while (*SourceStart && (*SourceStart != '=')) ++SourceStart;
      if (*SourceStart == '=') ++SourceStart;
      while (*SourceStart && isspace(*SourceStart)) ++SourceStart;
      //Find the end of the aliased type (before ';')
      const char *SourceEnd = SM.getCharacterData(SourceRange.getEnd());
      while (*SourceEnd && (*SourceEnd != ';')) ++SourceEnd;
      //Get the original text
      std::string OriginalType(SourceStart, SourceEnd - SourceStart);
      //Now we need to reconstruct the type with explicit default arguments
      std::string NewType = CTD->getNameAsString();
      NewType += "<";
      for (unsigned i = 0; i < TemplateArgs.size(); ++i) {
        if (i > 0) NewType += ", ";
        //Check if this parameter has a default argument
        if (i < ClassTP->size()) {
          auto *P = ClassTP->getParam(i);
          if (P->hasDefaultArg()) {
            //Use the default argument
            std::string DefaultArgStr;
            llvm::raw_string_ostream OS(DefaultArgStr);
            P->getDefaultArg()->print(OS, Result.Context->getPrintingPolicy());
            OS.flush();
            NewType += DefaultArgStr;
          } else {
            //Keep the original argument
            std::string ArgStr;
            llvm::raw_string_ostream OS(ArgStr);
            TemplateArgs[i].print(Result.Context->getPrintingPolicy(), OS, false);
            OS.flush();
            NewType += ArgStr;
          }
        } else {
          //Keep the original argument
          std::string ArgStr;
          llvm::raw_string_ostream OS(ArgStr);
          TemplateArgs[i].print(Result.Context->getPrintingPolicy(), OS, false);
          OS.flush();
          NewType += ArgStr;
        }
      }
      NewType += ">";
      //Replace the original type with the new one
      SourceLocation StartLoc = EqualLoc.getLocWithOffset(SourceStart - SM.getCharacterData(EqualLoc));
      SourceLocation EndLoc = EqualLoc.getLocWithOffset(SourceEnd - SM.getCharacterData(EqualLoc));
      Rewrite.ReplaceText(SourceRange(StartLoc, EndLoc), NewType);
    }
}
  
void MutatorFrontendAction_52::MutatorASTConsumer_52::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = typeAliasTemplateDecl().bind("TypeAliasTemplateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}