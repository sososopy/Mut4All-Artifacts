//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Template_Function_In_Packed_Struct_312
 */ 
class MutatorFrontendAction_Modify_Template_Function_In_Packed_Struct_312 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Modify_Template_Function_In_Packed_Struct_312)

private:
    class MutatorASTConsumer_Modify_Template_Function_In_Packed_Struct_312 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Modify_Template_Function_In_Packed_Struct_312(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Modify_Template_Function_In_Packed_Struct_312.h"

// ========================================================================================================
#define MUT312_OUTPUT 1

void MutatorFrontendAction_Modify_Template_Function_In_Packed_Struct_312::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("PackedStruct")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->hasAttr(clang::attr::Packed)) 
        return;
      //Get the source code text of target node
      auto methods = MT->methods();
      for (auto method : methods) {
        if (method->isTemplated()) {
          auto templateParams = method->getTemplateParameterList();
          if (templateParams) {
            auto param = templateParams->getParam(0);
            if (param) {
              auto type = param->getType();
              if (type->isPointerType()) {
                //Perform mutation on the source code text by applying string replacement
                auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                           method->getSourceRange());
                auto newType = "existing_type";
                std::string str = declaration;
                size_t pos = str.find(type.getAsString());
                if (pos != std::string::npos) {
                  str.replace(pos, type.getAsString().size(), newType);
                }
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(method->getSourceRange()), str);
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_Modify_Template_Function_In_Packed_Struct_312::MutatorASTConsumer_Modify_Template_Function_In_Packed_Struct_312::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAttribute(attr::Packed)).bind("PackedStruct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}