//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Parameter_Mismatch_In_Member_Function_Template_Specialization_186
 */ 
class MutatorFrontendAction_186 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(186)

private:
    class MutatorASTConsumer_186 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_186(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateSpecializationDecl*> classSpecs;
        std::vector<const clang::TemplateTypeParmDecl*> templateParams;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT186_OUTPUT 1

void MutatorFrontendAction_186::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FS = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncSpec")) {
      //Filter nodes in header files
      if (!FS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FS->getLocation()))
        return;
      //Check if this is an explicit specialization of a member function template
      if (!FS->isTemplateSpecialization() || !FS->getMemberSpecializationInfo())
        return;
      //Get the source code text of target node
      auto funcRange = FS->getSourceRange();
      if (!funcRange.isValid())
        return;
      auto funcText = stringutils::rangetoStr(*(Result.SourceManager), funcRange);
      //(optional)Record the node information to be used in the mutation process
      //Perform mutation on the source code text by applying string replacement
      int choice = getrandom::getRandomIndex(1);
      if (choice == 0) {
        //Option 1: Change the template template argument in the enclosing class specialization
        //Find the class template specialization
        auto classSpec = dyn_cast<ClassTemplateSpecializationDecl>(FS->getDeclContext());
        if (!classSpec)
          return;
        //Get the template arguments
        auto templateArgs = classSpec->getTemplateArgs();
        if (templateArgs.size() == 0)
          return;
        //Create a new template argument
        string newTemplateName = "DifferentTemplate";
        //Check if we have recorded any template parameters
        if (!templateParams.empty()) {
          size_t idx = getrandom::getRandomIndex(templateParams.size() - 1);
          newTemplateName = templateParams[idx]->getNameAsString();
        }
        //Replace the template argument in the function specialization
        size_t templateStart = funcText.find("template <> template");
        if (templateStart == string::npos)
          return;
        size_t classSpecStart = funcText.find(classSpec->getNameAsString());
        if (classSpecStart == string::npos)
          return;
        size_t openAngle = funcText.find('<', classSpecStart);
        if (openAngle == string::npos)
          return;
        size_t closeAngle = funcText.find('>', openAngle);
        if (closeAngle == string::npos)
          return;
        string oldTemplateArg = funcText.substr(openAngle + 1, closeAngle - openAngle - 1);
        funcText.replace(openAngle + 1, closeAngle - openAngle - 1, newTemplateName);
        //Insert new template declaration if needed
        string newDecl = "template <typename> struct " + newTemplateName + " {};\n";
        Rewrite.InsertTextBefore(funcRange.getBegin(), newDecl);
      } else {
        //Option 2: Change the function parameter type
        auto param = FS->getParamDecl(0);
        if (!param)
          return;
        auto paramRange = param->getSourceRange();
        if (!paramRange.isValid())
          return;
        auto paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
        //Replace with concrete type or different template
        string newParamType = "int";
        int typeChoice = getrandom::getRandomIndex(2);
        if (typeChoice == 0) {
          newParamType = "int";
        } else if (typeChoice == 1) {
          newParamType = "DifferentType<U>";
          //Insert declaration if needed
          string newDecl = "template <typename> struct DifferentType {};\n";
          Rewrite.InsertTextBefore(funcRange.getBegin(), newDecl);
        } else {
          newParamType = "TP<U>";
        }
        funcText.replace(funcText.find(paramText), paramText.length(), newParamType);
      }
      funcText = "/*mut186*/" + funcText;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(funcRange), funcText);
    }
    else if (auto *CS = Result.Nodes.getNodeAs<clang::ClassTemplateSpecializationDecl>("ClassSpec")) {
      if (!CS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CS->getLocation()))
        return;
      classSpecs.push_back(CS);
    }
    else if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParam")) {
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      templateParams.push_back(TP);
    }
}
  
void MutatorFrontendAction_186::MutatorASTConsumer_186::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //Match explicit specializations of member function templates
    DeclarationMatcher funcMatcher = functionDecl(
        isExplicitSpecialization(),
        hasParent(classTemplateSpecializationDecl()),
        unless(isImplicit())
    ).bind("FuncSpec");
    //Also match class template specializations for context
    DeclarationMatcher classMatcher = classTemplateSpecializationDecl().bind("ClassSpec");
    //Match template parameters for potential reuse
    DeclarationMatcher paramMatcher = templateTypeParmDecl().bind("TemplateParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.addMatcher(paramMatcher, &callback);
    matchFinder.matchAST(Context);
}