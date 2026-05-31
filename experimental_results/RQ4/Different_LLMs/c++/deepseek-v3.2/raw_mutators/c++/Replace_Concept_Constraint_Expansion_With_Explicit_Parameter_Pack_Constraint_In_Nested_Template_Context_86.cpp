//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Constraint_Expansion_With_Explicit_Parameter_Pack_Constraint_In_Nested_Template_Context_86
 */ 
class MutatorFrontendAction_86 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(86)

private:
    class MutatorASTConsumer_86 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_86(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl*> targetTemplates;
    };
};

//source file
#include "../include/Mutator_Replace_Concept_Constraint_Expansion_With_Explicit_Parameter_Pack_Constraint_In_Nested_Template_Context_86.h"

// ========================================================================================================
#define MUT86_OUTPUT 1

void MutatorFrontendAction_86::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Record candidate class templates for later mutation
      targetTemplates.push_back(CTD);
    }
    else if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("MemberFuncTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;
      
      //Find the parent class template of this member function template
      const clang::CXXRecordDecl* parentRecord = dyn_cast<clang::CXXRecordDecl>(FTD->getDeclContext());
      if (!parentRecord) return;
      
      const clang::ClassTemplateDecl* parentTemplate = nullptr;
      for (auto* CTD : targetTemplates) {
        if (CTD->getTemplatedDecl() == parentRecord) {
          parentTemplate = CTD;
          break;
        }
      }
      if (!parentTemplate) return;
      
      //Check if the function template has a parameter pack with concept constraint
      const clang::TemplateParameterList* funcParams = FTD->getTemplateParameters();
      if (!funcParams || funcParams->size() != 1) return;
      
      auto* templateParam = funcParams->getParam(0);
      if (!templateParam->isParameterPack()) return;
      
      //Check for concept constraint (simplified check)
      std::string paramStr;
      llvm::raw_string_ostream paramStream(paramStr);
      templateParam->print(paramStream, *Result.Context, clang::PrintingPolicy(Result.Context->getLangOpts()));
      paramStream.flush();
      
      if (paramStr.find("always_true") == std::string::npos) return;
      
      //Get the class template parameters
      const clang::TemplateParameterList* classParams = parentTemplate->getTemplateParameters();
      if (!classParams || !classParams->getParam(0)->isParameterPack()) return;
      
      //Get the function declaration
      clang::FunctionDecl* funcDecl = FTD->getTemplatedDecl();
      if (!funcDecl) return;
      
      //Get source ranges
      SourceRange funcTemplateRange = FTD->getSourceRange();
      SourceRange funcDeclRange = funcDecl->getSourceRange();
      
      if (!funcTemplateRange.isValid() || !funcDeclRange.isValid()) return;
      
      //Get original source code
      std::string originalTemplate = stringutils::rangetoStr(*(Result.SourceManager), funcTemplateRange);
      std::string originalFunc = stringutils::rangetoStr(*(Result.SourceManager), funcDeclRange);
      
      //Generate new template parameters (simulate expansion for 3 types as example)
      std::string newTemplateParams = "template <always_true<T> U0, always_true<T> U1, always_true<T> U2>";
      std::string newFuncParams = "(U0&& value0, U1&& value1, U2&& value2)";
      
      //Replace template parameter declaration
      size_t templateStart = originalTemplate.find("template");
      size_t templateEnd = originalTemplate.find(">", templateStart) + 1;
      if (templateEnd == std::string::npos) return;
      
      std::string mutatedTemplate = originalTemplate;
      mutatedTemplate.replace(templateStart, templateEnd - templateStart, newTemplateParams);
      
      //Replace function parameters
      size_t funcParamStart = originalFunc.find("(");
      size_t funcParamEnd = originalFunc.find(")", funcParamStart) + 1;
      if (funcParamEnd == std::string::npos) return;
      
      std::string mutatedFunc = originalFunc;
      mutatedFunc.replace(funcParamStart, funcParamEnd - funcParamStart, newFuncParams);
      
      //Replace the body parameter usage (simplified - replace pack expansion with individual parameters)
      //This is a simplified implementation that assumes specific patterns
      size_t bodyStart = mutatedFunc.find("{");
      if (bodyStart != std::string::npos) {
        std::string body = mutatedFunc.substr(bodyStart);
        //Replace values... with value0, value1, value2
        size_t packPos = body.find("values...");
        if (packPos != std::string::npos) {
          body.replace(packPos, 9, "value0, value1, value2");
          mutatedFunc = mutatedFunc.substr(0, bodyStart) + body;
        }
      }
      
      //Combine the mutated code
      std::string mutatedCode = mutatedTemplate + " " + mutatedFunc;
      mutatedCode = "/*mut86*/" + mutatedCode;
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(funcTemplateRange), mutatedCode);
    }
}
  
void MutatorFrontendAction_86::MutatorASTConsumer_86::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher classTemplateMatcher = classTemplateDecl().bind("ClassTemplate");
    DeclarationMatcher memberFuncMatcher = functionTemplateDecl(
        isExpansionInMainFile(),
        ofClass(classTemplateDecl().bind("ClassTemplate"))
    ).bind("MemberFuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(classTemplateMatcher, &callback);
    matchFinder.addMatcher(memberFuncMatcher, &callback);
    matchFinder.matchAST(Context);
}