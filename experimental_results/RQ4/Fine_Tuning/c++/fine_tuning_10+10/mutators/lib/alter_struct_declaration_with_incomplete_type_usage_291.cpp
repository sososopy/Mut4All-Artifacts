//source file
#include "../include/alter_struct_declaration_with_incomplete_type_usage_291.h"

// ========================================================================================================
#define MUT291_OUTPUT 1

void MutatorFrontendAction_291::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *StructDecl = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("TargetStruct")) {
        //Filter nodes in header files
        if (!StructDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                StructDecl->getLocation()))
            return;
        
        // Record the struct declaration
        if (StructDecl->isCompleteDefinition() && StructDecl->isStruct()) {
            TargetStruct = StructDecl;
        }
    } else if (auto *FuncDecl = Result.Nodes.getNodeAs<clang::FunctionDecl>("TargetFunction")) {
        //Filter nodes in header files
        if (!FuncDecl || !Result.Context->getSourceManager().isWrittenInMainFile(
                FuncDecl->getLocation()))
            return;

        if (TargetStruct && FuncDecl->hasBody()) {
            //Get the source code text of target node
            auto funcBody = stringutils::rangetoStr(*(Result.SourceManager), FuncDecl->getBody()->getSourceRange());
            std::string structName = TargetStruct->getNameAsString();
            std::string mutation = "struct " + structName + "* ptr; /*mut291*/\n";
            mutation += "ptr->member = 0; // Undefined behavior if member is accessed\n";
            
            //Perform mutation on the source code text by applying string replacement
            funcBody.insert(funcBody.find("{") + 1, mutation);
            
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FuncDecl->getBody()->getSourceRange()), funcBody);
        }
    }
}
  
void MutatorFrontendAction_291::MutatorASTConsumer_291::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher structMatcher = cxxRecordDecl(isStruct()).bind("TargetStruct");
    DeclarationMatcher funcMatcher = functionDecl(hasBody(stmt())).bind("TargetFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}