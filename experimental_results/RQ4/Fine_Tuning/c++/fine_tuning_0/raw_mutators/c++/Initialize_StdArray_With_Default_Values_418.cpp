//header file
#pragma once
#include "Mutator_base.h"

/**
 * Initialize_StdArray_With_Default_Values_418
 */ 
class MutatorFrontendAction_418 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(418)

private:
    class MutatorASTConsumer_418 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_418(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Initialize_StdArray_With_Default_Values_418.h"

// ========================================================================================================
#define MUT418_OUTPUT 1

void MutatorFrontendAction_418::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("StdArrayDecl")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->getType().getAsString().find("std::array") == string::npos)
        return;
      if (DL->hasInit() == false)
        return;
      auto inits = DL->getInit();
      if (inits->getType().getAsString().find("std::array") == string::npos)
        return;
      auto init_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              inits->getSourceRange());
      if (init_str != "{}")
        return;
      auto type = DL->getType();
      auto type_str = type.getAsString();
      string elem_type;
      int len = type_str.size();
      int i = 0;
      while (i < len && type_str[i] != '<')
        i++;
      i++;
      while (i < len && type_str[i] != ',') {
        elem_type += type_str[i];
        i++;
      }
      i++;
      string elem_num;
      while (i < len && type_str[i] != '>') {
        elem_num += type_str[i];
        i++;
      }
      string new_init = "{";
      int elem_num_int = stoi(elem_num);
      for (int i = 0; i < elem_num_int; i++) {
        if (elem_type == "int" || elem_type == "long long" ||
            elem_type == "long" || elem_type == "short" ||
            elem_type == "unsigned int" || elem_type == "unsigned long long" ||
            elem_type == "unsigned long" || elem_type == "unsigned short")
          new_init += "0";
        else
          new_init += elem_type + "()";
        if (i != elem_num_int - 1)
          new_init += ",";
      }
      new_init += "}";
      llvm::outs() << new_init << '\n';
      Rewrite.ReplaceText(inits->getSourceRange(), new_init);
    }
}
  
void MutatorFrontendAction_418::MutatorASTConsumer_418::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl().bind("StdArrayDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}