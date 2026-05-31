//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Consteval_CTAD_Class_Template_73
 */ 
class MutatorFrontendAction_73 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(73)

private:
    class MutatorASTConsumer_73 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_73(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl*> targetClasses;
        std::vector<std::string> classNames;
    };
};

//source file
#include "../include/Mutator_Duplicate_Consteval_CTAD_Class_Template_73.h"

// ========================================================================================================
#define MUT73_OUTPUT 1

void MutatorFrontendAction_73::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CTADClass")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      if (!MT->isTemplateDecl())
        return;
      auto *TemplateDecl = MT->getDescribedClassTemplate();
      if (!TemplateDecl)
        return;
      // Check for consteval constructor
      bool hasConstevalConstructor = false;
      for (auto *Method : MT->methods()) {
        if (Method->isConsteval() && isa<clang::CXXConstructorDecl>(Method)) {
          if (Method->getNumParams() == 1) {
            auto ParamType = Method->getParamDecl(0)->getType();
            if (ParamType.getAsString() == MT->getNameAsString() + "::F" ||
                ParamType.getAsString() == "F") {
              hasConstevalConstructor = true;
              break;
            }
          }
        }
      }
      if (!hasConstevalConstructor)
        return;
      //Get the source code text of target node
      auto classSource = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      auto className = MT->getNameAsString();
      //Record the node information to be used in the mutation process
      targetClasses.push_back(MT);
      classNames.push_back(className);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("CTADVar")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      if (!VD->isConstexpr())
        return;
      auto Type = VD->getType();
      std::string typeStr = Type.getAsString();
      if (typeStr.find("struct") == std::string::npos)
        return;
      // Find matching class template
      for (size_t i = 0; i < targetClasses.size(); ++i) {
        if (VD->getType().getAsString().find(classNames[i]) != std::string::npos) {
          //Perform mutation on the source code text by applying string replacement
          std::string newClassName = classNames[i] + "2";
          // Check if name already exists, increment if needed
          for (const auto& name : classNames) {
            if (name == newClassName) {
              int counter = 3;
              while (true) {
                newClassName = classNames[i] + std::to_string(counter);
                bool exists = false;
                for (const auto& n : classNames) {
                  if (n == newClassName) {
                    exists = true;
                    break;
                  }
                }
                if (!exists) break;
                counter++;
              }
            }
          }
          // Duplicate class template
          std::string duplicateClass = "\ntemplate<class F>\nstruct " + newClassName + " {\n    consteval " + newClassName + "(F) {}\n};\n";
          // Add new constexpr variable using CTAD with lambda
          std::string newVar = "constexpr " + newClassName + " var_" + newClassName + " = [](){};\n";
          std::string mutation = duplicateClass + newVar;
          mutation = "/*mut73*/" + mutation;
          //Replace the original AST node with the mutated one
          Rewrite.InsertTextAfterToken(VD->getEndLoc(), mutation);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_73::MutatorASTConsumer_73::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classMatcher = cxxRecordDecl(isClassTemplate(), hasDefinition()).bind("CTADClass");
    DeclarationMatcher varMatcher = varDecl(isConstexpr()).bind("CTADVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.matchAST(Context);
}