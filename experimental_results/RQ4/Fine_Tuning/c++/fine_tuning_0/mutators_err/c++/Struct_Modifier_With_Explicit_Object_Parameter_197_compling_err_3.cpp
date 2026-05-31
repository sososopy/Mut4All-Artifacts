//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Modifier_With_Explicit_Object_Parameter_197
 */ 
class MutatorFrontendAction_197 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(197)

private:
    class MutatorASTConsumer_197 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_197(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Struct_Modifier_With_Explicit_Object_Parameter_197.h"

// ========================================================================================================
#define MUT197_OUTPUT 1

void MutatorFrontendAction_197::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ST = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
      if (!ST || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ST->getLocation()))
        return;
      if (!ST->isStruct())
        return;
      auto methods = ST->methods();
      for (auto method : methods) {
        if (method->isOverloadedOperator()) {
          auto op_name = method->getNameAsString();
          llvm::outs() << op_name << '\n';
          if (op_name == "operator=" || op_name == "operator->" ||
              op_name == "operator[]")
            return;
          if (method->getNumParams() != 1)
            return;
          auto param = method->getParamDecl(0);
          if (param->getType().getAsString().find("const") == string::npos)
            return;
          auto method_str = stringutils::rangetoStr(
              *(Result.SourceManager), method->getSourceRange());
          llvm::outs() << method_str << '\n';
          if (method_str.find("default") != string::npos)
            return;
          if (method_str.find("bool") != string::npos) {
            method_str = stringutils::replace_first_of(method_str, "bool",
                                                       "int");
          }
          if (method_str.find("int") != string::npos) {
            method_str = stringutils::replace_first_of(method_str, "int",
                                                       "float");
          }
          if (method_str.find("float") != string::npos) {
            method_str = stringutils::replace_first_of(method_str, "float",
                                                       "double");
          }
          method_str += "=default;";
          method_str = "/*mut197*/" + method_str;
          Rewrite.ReplaceText(method->getSourceRange(), method_str);
        }
      }
    }
}
  
void MutatorFrontendAction_197::MutatorASTConsumer_197::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl().bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}