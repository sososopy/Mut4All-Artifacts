//header file
#pragma once
#include "Mutator_base.h"

/**
 * Duplicate_Deduction_Guide_Across_Modules_445
 */ 
class MutatorFrontendAction_445 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(445)

private:
    class MutatorASTConsumer_445 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_445(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl*> templates;
        std::vector<std::string> templateNames;
    };
};

//source file
#include "../include/Duplicate_Deduction_Guide_Across_Modules_445.h"

// ========================================================================================================
#define MUT445_OUTPUT 1

void MutatorFrontendAction_445::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Template")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      //Record the node information to be used in the mutation process
      templates.push_back(MT);
      templateNames.push_back(MT->getNameAsString());
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Record")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Perform mutation on the source code text by applying string replacement
      if (templates.empty()) {
        // If no template exists, create a simple class template with deduction guide
        std::string templateName = "Mut445_Template";
        std::string headerContent = "#pragma once\n\ntemplate<typename T> struct " + templateName + " { " + templateName + "(T); };\ntemplate<typename T> " + templateName + "(T) -> " + templateName + "<T>;\n";
        std::string moduleA = "export module ModuleA;\nexport import \"" + templateName + ".h\";\n";
        std::string moduleB = "export module ModuleB;\nimport ModuleA;\n#include \"" + templateName + ".h\"\n";
        std::string mainContent = "import ModuleA;\nimport ModuleB;\n";
        Rewrite.InsertText(MT->getEndLoc(), "\n/*mut445*/" + headerContent + moduleA + moduleB + mainContent);
      } else {
        // Use existing template
        size_t index = getrandom::getRandomIndex(templates.size() - 1);
        const clang::ClassTemplateDecl* targetTemplate = templates[index];
        std::string templateName = templateNames[index];
        std::string headerContent = "#pragma once\n\ntemplate<typename T> struct " + templateName + " { " + templateName + "(T); };\ntemplate<typename T> " + templateName + "(T) -> " + templateName + "<T>;\n";
        std::string moduleA = "export module ModuleA;\nexport import \"" + templateName + ".h\";\n";
        std::string moduleB = "export module ModuleB;\nimport ModuleA;\n#include \"" + templateName + ".h\"\n";
        std::string mainContent = "import ModuleA;\nimport ModuleB;\n";
        Rewrite.InsertText(MT->getEndLoc(), "\n/*mut445*/" + headerContent + moduleA + moduleB + mainContent);
      }
      //Replace the original AST node with the mutated one
    }
}
  
void MutatorFrontendAction_445::MutatorASTConsumer_445::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("Template");
    DeclarationMatcher recordMatcher = cxxRecordDecl().bind("Record");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}