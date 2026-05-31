//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Templated_Member_Function_458
 */ 
class MutatorFrontendAction_458 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(458)

private:
    class MutatorASTConsumer_458 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_458(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl*> templateClasses;
        std::vector<std::string> templateParamNames;
        std::vector<const clang::FunctionDecl*> memberFunctions;
    };
};

//source file
#include "../include/Mutator_Specialize_Templated_Member_Function_458.h"

// ========================================================================================================
#define MUT458_OUTPUT 1

void MutatorFrontendAction_458::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("TemplateClass")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      if (!CTD->isThisDeclarationADefinition())
        return;
      //Record the node information to be used in the mutation process
      templateClasses.push_back(CTD);
      // Extract template parameter names
      auto *TP = CTD->getTemplateParameters();
      if (TP) {
        std::string paramName;
        for (unsigned i = 0; i < TP->size(); ++i) {
          auto *P = TP->getParam(i);
          if (P->isTemplateParameter()) {
            if (auto *TTP = dyn_cast<TemplateTemplateParmDecl>(P)) {
              paramName = TTP->getNameAsString();
              if (paramName.empty()) paramName = "T" + std::to_string(i);
              templateParamNames.push_back(paramName);
            }
          }
        }
      }
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("MemberFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isCXXClassMember())
        return;
      // Check if function is templated and has default argument
      if (!FD->getDescribedFunctionTemplate())
        return;
      auto *FTD = FD->getDescribedFunctionTemplate();
      if (FTD->getTemplateParameters()->size() == 0)
        return;
      // Check for default argument
      if (FD->getNumParams() == 0 || !FD->getParamDecl(0)->hasDefaultArg())
        return;
      //Record the node information to be used in the mutation process
      memberFunctions.push_back(FD);
    }
    else if (auto *CXXRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassInstance")) {
      //Filter nodes in header files
      if (!CXXRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CXXRD->getLocation()))
        return;
      if (!CXXRD->isTemplateDecl())
        return;
      // Get the source code text of target node
      if (templateClasses.empty() || memberFunctions.empty())
        return;
      // Choose random template class and member function
      size_t classIdx = getrandom::getRandomIndex(templateClasses.size() - 1);
      size_t funcIdx = getrandom::getRandomIndex(memberFunctions.size() - 1);
      auto *CTD = templateClasses[classIdx];
      auto *FD = memberFunctions[funcIdx];
      // Ensure they belong to same class
      if (FD->getParent() != CTD->getTemplatedDecl())
        // Perform mutation on the source code text by applying string replacement
        // Generate specialization
        std::string specialization;
        std::string className = CTD->getNameAsString();
        // Get template arguments for specialization
        std::string templateArgs;
        if (!templateParamNames.empty()) {
          size_t paramIdx = getrandom::getRandomIndex(templateParamNames.size() - 1);
          std::string paramName = templateParamNames[paramIdx];
          templateArgs = paramName;
        } else {
          templateArgs = "T";
        }
        // Get function template parameters
        auto *FTD = FD->getDescribedFunctionTemplate();
        std::string funcTemplateParams;
        auto *FTP = FTD->getTemplateParameters();
        if (FTP->size() > 0) {
          auto *P = FTP->getParam(0);
          funcTemplateParams = P->getNameAsString();
          if (funcTemplateParams.empty()) funcTemplateParams = "U";
        } else {
          funcTemplateParams = "U";
        }
        // Generate mismatched parameter type (pointer instead of original)
        std::string mismatchedType = templateArgs + "<" + funcTemplateParams + ">*";
        std::string funcName = FD->getNameAsString();
        specialization = "template <> template <typename " + funcTemplateParams + "> void " + className + "<" + templateArgs + ">::" + funcName + "(" + mismatchedType + ") {}";
        // Insert specialization after class definition
        SourceLocation insertLoc = CTD->getEndLoc();
        // Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(insertLoc, "\n/*mut458*/" + specialization + "\n");
        // Also add a call to the function without argument to use default argument
        std::string callCode = className + "<" + templateArgs + "> instance_458;\ninstance_458." + funcName + "<int>();\n";
        Rewrite.InsertTextAfterToken(insertLoc, "\n" + callCode);
    }
}
  
void MutatorFrontendAction_458::MutatorASTConsumer_458::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateClassMatcher = classTemplateDecl().bind("TemplateClass");
    DeclarationMatcher memberFunctionMatcher = functionDecl(isTemplate(), hasDefaultArg(0), isMember()).bind("MemberFunction");
    DeclarationMatcher classInstanceMatcher = cxxRecordDecl(isTemplateDecl()).bind("ClassInstance");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateClassMatcher, &callback);
    matchFinder.addMatcher(memberFunctionMatcher, &callback);
    matchFinder.addMatcher(classInstanceMatcher, &callback);
    matchFinder.matchAST(Context);
}